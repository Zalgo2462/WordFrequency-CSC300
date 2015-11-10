/*
Authors: John Weiss Ph.D., Logan Lembke, Brady Shimp
*/
#ifndef _TOKENIZE_H_
#define _TOKENIZE_H_
#include <string>
#include <vector>
#include <iostream>

const char* VALID = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'";

void tokenize( const std::string& str, std::vector<std::string>& tokens, const std::string& valid = " " );

#endif
