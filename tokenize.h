/*
Authors: John Weiss Ph.D., Logan Lembke
*/
#ifndef _TOKENIZE_H_
#define _TOKENIZE_H_
#include <string>
#include <vector>
#include <iostream>

const char* DELIMITERS = " ,.*!";
const char* VALID = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

void tokenize1( const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " " );
void tokenize2( const std::string& str, std::vector<std::string>& tokens, const std::string& valid = " " );

#endif
