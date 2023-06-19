set hello => "hello";
set world => "world!";

log => hello, " ";
logl => world;

set hw => "hello world! but another way.";
logl => hw;
logl => "";

logl => "or you can just log a string without defining a variable.";
log => "you can log a string... ", "and another string.";
logl => "";