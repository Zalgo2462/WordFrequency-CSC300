/*
Authors: John Weiss Ph.D., Logan Lembke, Brady Shimp
*/
#ifndef _TOKENIZE_H_
#define _TOKENIZE_H_
#include <string>
#include <vector>
#include <iostream>

void tokenize( const std::string& str, std::vector<std::string>& tokens, const std::string& valid = " " );
void processTokens( std::vector<std::string> & tokens );

#endif
