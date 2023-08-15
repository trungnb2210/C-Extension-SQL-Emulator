# Define ANSI color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No color

# Remote server details
db_user="lab"
db_name="mondial"
db_password="lab"
db_host="db"

for test_file in generated_test/queries/*.txt; do
    file_name=$(basename "$test_file")

    #generate SQL from Peter's Database
    cp "$test_file" "generated_test/psql_syntax_queries/$file_name"

    #change syntax
    sed -i 's/==/=/g' "generated_test/psql_syntax_queries/$file_name" 

    #access database and run query
    export PGPASSWORD="$db_password"
    sql_query=$(cat "generated_test/psql_syntax_queries/$file_name")
    output_file="output_${file_name}"
    psql_output=$(psql -h "$db_host" -U "$db_user" -d "$db_name" -c "$sql_query")
    echo "$psql_output" > "generated_test/expected/$output_file"
    unset PGPASSWORD

    #generate SQL from SQL--
    ../sql "integration_test_tables.txt" "$test_file" "result.txt"
    cp "result.txt" "generated_test/actual/${file_name}"

    #compare and test
    stripped_output=$(cat "generated_test/expected/output_$file_name" | head -n -1 | sed 's/\.[0-9][0-9]//g' | tr -d '[:space:][:punct:]' | tr '[:upper:]' '[:lower:]')
    stripped_result=$(cat "generated_test/actual/$file_name" | sed 's/([^)]*)//g' | tail -n +2 | tr -d '[:space:][:punct:]' | tr '[:upper:]' '[:lower:]') 

    sleep 0.002

    if [ "$stripped_result" = "$stripped_output" ]; then
        echo -e "Test $file_name: ${GREEN}PASSED${NC}"
    else
        echo -e "Test $file_name: ${RED}FAILED${NC}"
    fi

done
echo "-----------------------------------"

rm "./result.txt"
