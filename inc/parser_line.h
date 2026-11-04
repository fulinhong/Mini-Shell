#ifndef PARSER_LINE_H
#define PARSER_LINE_H

#define PARSER_LINE_MAX_TOKENS 128

// 修改外部char *argv[PARSER_LINE_MAX_TOKENS];
// 把命令行解析为参数数组，返回参数数量, -1表示解析失败
int ParseLine(char *line, char **argv);

#endif