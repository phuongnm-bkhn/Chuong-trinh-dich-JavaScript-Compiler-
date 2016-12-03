%{
#include<stdio.h>
#include<string.h>
#include"bai3.tab.h"

%}

condition 	(==|!=|>=|<=|>|<)
dig		[0-9]
ws		[ \t]+
alpha		[A-Za-z]
var_name	({alpha}|\$)({alpha}|{dig}|\_|\.|\-|\/|\$)*

id  		({alpha})({alpha}|{dig}|\_|\-)*

num1		[-+]?{dig}+\.?([eE][-+]?{dig}+)?
num2		[-+]?{dig}*\.{dig}+([eE][-+]?{dig}+)?
const_number	{num1}|{num2}

%%
[\t\r\n]
{ws}		/*  bỏ qua */
while		return WHILE; 
do		return DO; 
var  		return VAR;
"||"		return OR;
"&&"		return AND;
{condition}	return CONDITION;
{num1}		return NUMBER; 
{id}		return ID;
 .		return yytext[0];

%%