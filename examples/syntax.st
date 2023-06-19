set hstr => "hello";
log => hstr;

# function definition with arguments
fn my_function: arg1, arg2 => {
    set result = arg1 + arg2;
    ret result;
};

my_function => 1, 2; # function call with arguments

# empty argument function definition
fn another_function: => {
    log => "hello from another function";
};

another_function => NULL; # empty argument function call