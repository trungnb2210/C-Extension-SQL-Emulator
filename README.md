Submissions: Aboud Dabbas, Angus Leung, Malhar Rajpal, Trung Nguyen 

Part I - Emulator Implementation
Part II - Assembler Implementation
    run "make clean; make" in ./src to generate emulate, assemble executable
    run ./run -Epfe ../src/emulate   in ./armv8_testsuite   (to run emulator testsuite)
    run ./run -Apfa ../src/assemble  in ./armv8_testsuite   (to run assembler testsuite)

    emulate.c, assemble.c in ./src
    Other emulator code files are in ./src/emulate_files
    Other assembler code files are in ./src/assemble_files
    Shared files between emulator and assembler are in ./src

Part III - Raspberry Pi Assembly - ./programs/led_blink.s
    call "make clean; make" in ./src to generate assembly executable
    run ./assemble ../programs/led_blink.s ../programs/kernel8.img to generate kernel8.img for RPi

Part IV - Extension (SQL--)
    call "make clean; make" in ./extension to generate sql executable
    run ./unit_tests in ./extension/sql_files to run unit tests 

    run chmod +x testman.sh     (permissions)
    run chmod +x testgen.sh     (permissions)

    run ./testgen.sh in ./extension/sql_files to run integration tests against Professor McBrien's SQL Database
    run ./testman.sh in ./extension/sql_files to run integration tests against manually generated expected outputs

    sql.c in ./extension
    Other sql code files are in ./extension/sql_files
    ./extension/sql_files/integration_test_tables.txt is included for testman.sh to run integration tests

Interim Checkpoint Report and Final Report in ./doc 

IF YOU WANT TO TRY SQL QUERIES FOR YOURSELF: 
    There is an example table data set ./extension/USER_TEST_TABLE.txt 
    There is an example query          ./extension/USER_TEST_QUERY.txt
    run "make clean; make" in ./extension 
    run ./sql USER_TEST_TABLE.txt USER_TEST_QUERY.txt USER_TEST_RESULT.txt   in ./extension
    
    Look at the query result!

Enjoy!

