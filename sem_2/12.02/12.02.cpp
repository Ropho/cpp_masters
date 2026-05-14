#include <cstdio>
#include <cstddef>

char const * code[] = {
	"#include <cstdio>",
	"#include <cstddef>",
	"",
	"char const * code[] = {",
	"};",
	"",
	"void print_escaped(char const * text)",
	"{",
	"\tfor (char const * p = text; *p != '\\0'; ++p)",
	"\t{",
	"\t\tswitch (*p)",
	"\t\t{",
	"\t\tcase '\\\\':",
	"\t\t\tstd::printf(\"\\\\\\\\\");",
	"\t\t\tbreak;",
	"\t\tcase '\"':",
	"\t\t\tstd::printf(\"\\\\\\\"\");",
	"\t\t\tbreak;",
	"\t\tcase '\\t':",
	"\t\t\tstd::printf(\"\\\\t\");",
	"\t\t\tbreak;",
	"\t\tdefault:",
	"\t\t\tstd::printf(\"%c\", *p);",
	"\t\t\tbreak;",
	"\t\t}",
	"\t}",
	"}",
	"",
	"int main()",
	"{",
	"\tconstexpr std::size_t prefix_lines = 4;",
	"\tconstexpr std::size_t line_count = sizeof(code) / sizeof(code[0]);",
	"",
	"\tfor (std::size_t i = 0; i < prefix_lines; ++i)",
	"\t{",
	"\t\tstd::printf(\"%s\\n\", code[i]);",
	"\t}",
	"",
	"\tfor (std::size_t i = 0; i < line_count; ++i)",
	"\t{",
	"\t\tstd::printf(\"\\t\\\"\");",
	"\t\tprint_escaped(code[i]);",
	"\t\tstd::printf(\"\\\",\\n\");",
	"\t}",
	"",
	"\tfor (std::size_t i = prefix_lines; i < line_count; ++i)",
	"\t{",
	"\t\tstd::printf(\"%s\\n\", code[i]);",
	"\t}",
	"",
	"\treturn 0;",
	"}",
};

void print_escaped(char const * text)
{
	for (char const * p = text; *p != '\0'; ++p)
	{
		switch (*p)
		{
		case '\\':
			std::printf("\\\\");
			break;
		case '"':
			std::printf("\\\"");
			break;
		case '\t':
			std::printf("\\t");
			break;
		default:
			std::printf("%c", *p);
			break;
		}
	}
}

int main()
{
	constexpr std::size_t prefix_lines = 4;
	constexpr std::size_t line_count = sizeof(code) / sizeof(code[0]);

	for (std::size_t i = 0; i < prefix_lines; ++i)
	{
		std::printf("%s\n", code[i]);
	}

	for (std::size_t i = 0; i < line_count; ++i)
	{
		std::printf("\t\"");
		print_escaped(code[i]);
		std::printf("\",\n");
	}

	for (std::size_t i = prefix_lines; i < line_count; ++i)
	{
		std::printf("%s\n", code[i]);
	}

	return 0;
}
