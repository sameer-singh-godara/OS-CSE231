#include <stdio.h>
#include <stdlib.h>
#include<sys/mman.h>
#include<unistd.h>
#include<inttypes.h>


#define PAGE_SIZE 4096


typedef struct SubChainNode {
    size_t size;
    void* virtualAddress;
    int type;  // 0 for HOLE, 1 for PROCESS
    struct SubChainNode* right;
    struct SubChainNode* left;
    void* mem_ptr; // MeMS physical address
} SubChainNode;




typedef struct MainChainNode {
    void* virtualAddressStart;
    void* virtualAddressEnd;
    int pageNumber; 
    struct MainChainNode* up;
    struct MainChainNode* down;
    SubChainNode* sub_chain; // Doubly linked list for sub-chain
} MainChainNode;



void* mems_heap_start = NULL; // Declare and initialize mems_heap_start
MainChainNode* free_list_head = NULL; // Declare and initialize free_list_head

void mems_init() {

    mems_heap_start = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    // Handling case where memory not allocated
    if (mems_heap_start == MAP_FAILED) {

        // Using perror
        perror("mmap for mems_heap_start failed");
        exit(1);    // exiting
    }
    
    free_list_head = (MainChainNode*)mmap(NULL, sizeof(MainChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    // Handling case where memory not allocated
    if (free_list_head == MAP_FAILED) {

        // Using perror
        perror("mmap for free_list_head failed");
        exit(1);    // exiting
    }
    
    free_list_head->virtualAddressStart = 0;
    free_list_head->virtualAddressEnd = 0;
}




static int pii = 0;
static int lp = 0;
static uintptr_t mems_offset = 1000;
static int finish_used = 0;



void mems_finish() {
    MainChainNode* current_main = free_list_head;

    finish_used = 1;

    while (current_main != NULL) {
        SubChainNode* current_sub = current_main->sub_chain;

        while (current_sub != NULL) {
            if (current_sub->type == 1) { // Check if it's a PROCESS segment
                // Unmap the allocated memory
                if (current_sub->mem_ptr != NULL) {
                    if (munmap(current_sub->mem_ptr, current_sub->size) == -1) {
                        perror("munmap failed");
                        exit(1);
                    }
                }
            }

            SubChainNode* temp = current_sub;
            current_sub = current_sub->right;
            // Unmap the SubChainNode
            if (munmap(temp, sizeof(SubChainNode)) == -1) {
                perror("munmap failed");
                exit(1);
            }
        }

        MainChainNode* temp_main = current_main;
        current_main = current_main->down;
        // Unmap the MainChainNode
        if (munmap(temp_main, sizeof(MainChainNode)) == -1) {
            perror("munmap failed");
            exit(1);
        }
    }

    // Unmap the MeMS heap start
    if (mems_heap_start != NULL) {
        if (munmap(mems_heap_start, PAGE_SIZE) == -1) {
            perror("munmap failed");
            exit(1);
        }
    }

    // Unmap the free_list_head
    if (free_list_head != NULL) {
        if (munmap(free_list_head, sizeof(MainChainNode)) == -1) {
            perror("munmap failed");
            exit(1);
        }
    }
}





void* mems_malloc(size_t size) {
    size_t page_aligned_size = 0;
    MainChainNode* current_main = free_list_head;
    MainChainNode* prev_node = NULL;

    finish_used = 0;

    while (current_main != NULL) {
        SubChainNode* current_sub = current_main->sub_chain;
        SubChainNode* previous_sub = NULL;

        while (current_sub != NULL) {
            // printf("%lu this is virtual address in which node will be allocated %lu \n", (unsigned long) mems_offset, (unsigned long)current_sub->virtualAddress);
            if (current_sub->type == 0 && current_sub->size >= size && (unsigned long)current_sub->virtualAddress >= (unsigned long)mems_offset) {
                // Found a HOLE segment with enough space
                if (current_sub->size > size) {
                    // Split the HOLE into a new HOLE and a PROCESS segment
                     
                    SubChainNode* new_hole = (SubChainNode*)mmap(NULL, sizeof(SubChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                    new_hole->size = current_sub->size - size;
                    new_hole->type = 0;
                    new_hole->mem_ptr = NULL;

                    void* mem_ptr = mmap(NULL,size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                    if (mem_ptr == MAP_FAILED) {
                        perror("mmap failed");
                        exit(1);
                    }
                    
                    current_sub->size = size;
                    current_sub->type = 1; // Mark as PROCESS
                    
                    current_sub->mem_ptr = mem_ptr;

                    // Adjust the pointers in the sub-chain
                    new_hole->right = current_sub->right;
                    new_hole->left = current_sub;

                    // if (current_sub->right != NULL) {
                    //     current_sub->right->left = new_hole;
                    // }
                    current_sub->right = new_hole;

                    // if (previous_sub != NULL) {
                    //     previous_sub->right = new_hole;
                    // } 

                    // else {
                    //     current_main->sub_chain = new_hole;
                    // }

                    // Calculate the virtual address with the offset
                    mems_offset += size;
                    current_sub->virtualAddress = (void*)mems_offset;
                    
                    // printf("virtual address of %d \n", current_sub->virtualAddress);
                    void* mems_addr = (void*)(mems_offset);
                    new_hole->virtualAddress = mems_addr + size;
                    // printf("%lu VA of hole after spliting a big hole %lu\n", (unsigned long)new_hole->virtualAddress, (unsigned long)mems_offset);
                    // printf("%d va is \n", current_sub->virtualAddress);
                    // printf("%ld process \n %ld hole \n", current_sub->virtualAddress, mems_addr);
                    return mems_addr;
                } else {
                    // The HOLE is exactly the right size, so use it as is
                    current_sub->type = 1; // Mark as PROCESS
                    

                    void* mem_ptr = mmap(NULL,size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

                    if (mem_ptr == MAP_FAILED) {
                        perror("mmap");
                        exit(1);
                    }

                    current_sub->mem_ptr = mem_ptr;
                    current_sub->virtualAddress = (void *)mems_offset;
                    // Calculate the virtual address with the offset
                    mems_offset += size;
                    // printf("%d va is \n", current_sub->virtualAddress);
                    void* mems_addr = (void*)(mems_offset);
                    
                    return mems_addr;
                }
                
            } else if (current_sub->type == 0 && current_sub->size >= size && (unsigned long)current_sub->virtualAddress < (unsigned long)mems_offset) {
                // Found a HOLE segment with enough space
                if (current_sub->size > size) {
                    // Split the HOLE into a new HOLE and a PROCESS segment
                     
                    SubChainNode* new_hole = (SubChainNode*)mmap(NULL, sizeof(SubChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                    new_hole->size = current_sub->size - size;
                    new_hole->type = 0;
                    new_hole->mem_ptr = NULL;
                    new_hole->virtualAddress = current_sub->virtualAddress + size;

                    void* mem_ptr = mmap(NULL,size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                    if (mem_ptr == MAP_FAILED) {
                        perror("mmap failed");
                        exit(1);
                    }
                    
                    current_sub->size = size;
                    current_sub->type = 1; // Mark as PROCESS
                    
                    current_sub->mem_ptr = mem_ptr;

                    new_hole->right = current_sub->right;
                    new_hole->left = current_sub;
                    current_sub->right = new_hole;

                

                    return current_sub->virtualAddress;
                } else {
                    // The HOLE is exactly the right size, so use it as is
                    current_sub->type = 1; // Mark as PROCESS
                    

                    void* mem_ptr = mmap(NULL,size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

                    if (mem_ptr == MAP_FAILED) {
                        perror("mmap");
                        exit(1);
                    }

                    current_sub->mem_ptr = mem_ptr;
                    
                    return current_sub->virtualAddress;
                }
                
            }

            previous_sub = current_sub;
            current_sub = current_sub->right;
        }

        prev_node = current_main;
        current_main = current_main->down;
        
    }

    pii++;
    if (pii>1){
        mems_offset = (unsigned long) prev_node->virtualAddressEnd  + 1;
    }
    
    int i=1;
    while(size>=(i*PAGE_SIZE)){
        i++;
    }
    page_aligned_size = i*PAGE_SIZE;


    // If no suitable segment was found, request memory from the OS using mmap
    void* mem_ptr = mmap(NULL,size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem_ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Create a new main chain node and its sub-chain node for the allocated memory
    MainChainNode* new_main_node = (MainChainNode*)mmap(NULL, sizeof(MainChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    SubChainNode* new_process_node = (SubChainNode*)mmap(NULL, sizeof(SubChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    SubChainNode* new_hole_node = (SubChainNode*)mmap(NULL, sizeof(SubChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    new_main_node->virtualAddressStart = (void *)mems_offset;
    new_main_node->virtualAddressEnd = new_main_node->virtualAddressStart + page_aligned_size - 1;

    new_main_node->sub_chain = new_process_node;
    new_process_node->size = size;
    new_process_node->type = 1; // Mark as PROCESS
    new_process_node->mem_ptr = mem_ptr;

    new_hole_node->size = page_aligned_size - size;
    new_hole_node->type = 0;
    new_hole_node->mem_ptr = NULL;

    // Update pointers in the sub-chain
    new_process_node->right = new_hole_node;
    new_process_node->left = NULL;
    new_hole_node->right = NULL;
    new_hole_node->left = new_process_node;

    // Update pointers in the main chain
    new_main_node->down = NULL;
    if (prev_node != NULL) {
        prev_node->down = new_main_node;
    } else {
        free_list_head = new_main_node;
    }
    // printf("virtual address of %d \n", new_process_node->virtualAddress);


    // mems_offset += size;
    new_process_node->virtualAddress = (void *)mems_offset;

    
    // printf("%d va is \n", new_process_node->virtualAddress);
    void* mems_addr = (void*)(mems_offset);
    new_hole_node->virtualAddress = mems_addr + size;
    // printf("%lu VA of hole after spliting new page\n", (unsigned long)new_hole_node->virtualAddress);
    // printf("%lu VA intial of new page\n", (unsigned long)new_main_node->virtualAddressStart);
    // printf("%lu VA end of new page\n", (unsigned long)new_main_node->virtualAddressEnd);
    return mems_addr;
}






void printMainChain(MainChainNode *mainNode)
{

    if (mainNode == NULL)
    {
        return;
    }

    SubChainNode *subNode = mainNode->sub_chain;
    printf("MAIN[%lu:%lu] -> ", (unsigned long)mainNode->virtualAddressStart, (unsigned long)mainNode->virtualAddressEnd);

    while (subNode != NULL)
    {

        if (subNode->type == 1)
        {
            printf("P[%lu:%lu]", (unsigned long)subNode->virtualAddress, (unsigned long)(subNode->virtualAddress + subNode->size - 1));
        }

        if (subNode->right != NULL)
        {
            printf(" <-> ");
        }
        else if (subNode->type == 0)
        {
            printf("H[%lu:%lu] <-> NULL\n", (unsigned long)subNode->virtualAddress  , (unsigned long)(subNode->virtualAddress + subNode->size-1));

            if (subNode->right != NULL)
            {
                printf(":%lu", (unsigned long)(subNode->virtualAddress + subNode->size + subNode->right->size - 1));
            }

            printf("\n");
        }
        subNode = subNode->right;
    }

    printMainChain(mainNode->down);
}





// Function to print MeMS system statistics
void mems_print_stats()
{
    if(finish_used == 1){

        printf("--------- MeMS System STATS --------\n");
        printf("Pages used: %d\n", 0);
        printf("Space unused: %d\n", 0);
        printf("Main Chain Length: %d\n", 0);
        printf("Allocate memory first");
    }

    else{
        int spaceUnused = 0;
        int mainChainLength = 0;
        int subChainLengthArray[100000]; // Adjust the size as needed

        for (int i = 0; i < 100000; i++)
        {
            subChainLengthArray[i] = 0;
        }

        MainChainNode *mainNode = free_list_head->down;

        int ind = 0;

        while (mainNode != NULL)
        {

            mainChainLength++;

            SubChainNode *subNode = mainNode->sub_chain;

            int pagesUsed = 0;

            while (subNode != NULL)
            {
                if (subNode->type == 0)
                {
                    spaceUnused += subNode->size;
                    pagesUsed++;
                }

                else if(subNode->type == 1)
                {
                    pagesUsed ++;
                }

                subNode = subNode->right;
            }

            // printf("pages USED : %d\n" , pagesUsed);

            subChainLengthArray[ind] = pagesUsed;
            ind++;

            mainNode = mainNode->down;
        }

        printf("--------- MeMS System STATS --------\n");
        printMainChain(free_list_head->down);
        printf("Pages used: %d\n", pii);
        printf("Space unused: %d\n", spaceUnused);
        printf("Main Chain Length: %d\n", mainChainLength);
        printf("Sub-chain Length array: [");

        for (int i = 0; i < 100000; i++)
        {
            if (subChainLengthArray[i] != 0)
            {
                printf("%d, ", subChainLengthArray[i]);
            }
        }

        printf("]\n");

        printf("----------------------\n");
    }
}






void* mems_get(void* v_ptr) {
    // Start from the free_list_head
    MainChainNode* main_chain_node = free_list_head;
    
    int i = 0;
    while (main_chain_node != NULL) {
        SubChainNode* sub_chain_node = main_chain_node->sub_chain;
        while (sub_chain_node != NULL) {
            // printf("%d --> %d - %d\n", i, sub_chain_node->virtualAddress, v_ptr);
            if (sub_chain_node->virtualAddress <= v_ptr && sub_chain_node->right->virtualAddress >= v_ptr){
                // printf("YES");
                int diff = v_ptr - sub_chain_node->virtualAddress;
                // printf("%d subchain -> memptr" , diff);
                return sub_chain_node->mem_ptr + diff ;
            }
            sub_chain_node = sub_chain_node->right;
            i++;
        }

        main_chain_node = main_chain_node->down;
    }

    // MeMS virtual address not found, return NULL
    return NULL;
}   





void mems_free(void* v_ptr) {
    MainChainNode* main_chain_node = free_list_head;
    
    while (main_chain_node != NULL) {
        SubChainNode* sub_chain_node = main_chain_node->sub_chain;
        while (sub_chain_node != NULL) {
            if (sub_chain_node->virtualAddress == v_ptr && sub_chain_node->right->type == 0 && sub_chain_node->right != NULL){
                SubChainNode* leftNode = sub_chain_node->left;
                SubChainNode* holeNode = sub_chain_node->right;
                size_t sizeNode = sub_chain_node->size + holeNode->size;

                holeNode->virtualAddress = v_ptr;
                holeNode->size = sizeNode;

                leftNode->right = holeNode;


                if (munmap(sub_chain_node, sub_chain_node->size) == -1) {
                    perror("munmap failed");
                    exit(1);
                }


                return;
            }
            else if (sub_chain_node->virtualAddress == v_ptr && sub_chain_node->right == NULL){
                SubChainNode* leftNode = sub_chain_node->left;
                leftNode->right = sub_chain_node->right;
                if (munmap(sub_chain_node, sub_chain_node->size) == -1) {
                    perror("munmap failed");
                    exit(1);
                }


                return;
            }
            sub_chain_node = sub_chain_node->right;
        }

        main_chain_node = main_chain_node->down;
    }
    printf("No Node with Such Address\n");
    return;
}