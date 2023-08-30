# Creating result directory if not present

if [ ! -d "Result" ]; then
    mkdir "Result"
fi

# Reading input from input.txt file

while IFS=" " read -r x y operation; do
    case "$operation" in
        "xor")
            result=$((x ^ y))
            ;;
        "product")
            result=$((x * y))
            ;;
        "compare")
            if [ "$x" -gt "$y" ]; then
                result="$x"
            else
                result="$y"
            fi
            ;;
        *)
            echo "Invalid operation: $operation"
            continue
            ;;
    esac
   
    # Writing result in output.txt file
   
    echo "Result of $operation $x $y: $result" >> Result/output.txt
   
done < input.txt
