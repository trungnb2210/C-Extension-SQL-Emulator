# Define ANSI color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No color


for test_file in manual_test/queries/*.txt; do
    file_name=$(basename "$test_file")

    #generate SQL from SQL--
    ../sql "integration_test_tables.txt" "$test_file" "result.txt"
    cp "result.txt" "manual_test/actual/${file_name}"

    #compare and test
    stripped_expected=$(cat "manual_test/expected/$file_name" | sed 's/([^)]*)//g' | tail -n +2 | tr -d '[:space:][:punct:]' | tr '[:upper:]' '[:lower:]') 
    stripped_result=$(cat "manual_test/actual/$file_name" | sed 's/([^)]*)//g' | tail -n +2 | tr -d '[:space:][:punct:]' | tr '[:upper:]' '[:lower:]') 

    sleep 0.002

    if [ "$stripped_result" = "$stripped_expected" ]; then
        echo -e "Test $file_name: ${GREEN}PASSED${NC}"
    else
        echo -e "Test $file_name: ${RED}FAILED${NC}"
    fi

done
echo "-----------------------------------"

rm "result.txt"
