#include "cdecl.h"
#include "src/toolset.h"

#define BUF_SZ     256
#define OUTPUT_SZ  1024

RUN(cdecl, "C declaration analysis tool")
{
	struct cdecl *c = cdecl_new();
	assert(c);
	int retcode;
	char input[BUF_SZ], output[OUTPUT_SZ];

	printf(">");
	while (fgets(input, BUF_SZ-1, stdin)) {
		output[0] = '\0';
		cdecl_input(c, input);
		retcode = cdecl_analyze(c, output, OUTPUT_SZ);
		if (0 == retcode) {
			if (strlen(output) > 0) {
				char *ptr = output+strlen(output)-1;
				while (' ' == *ptr)
					*ptr-- = '\0';
			}
			printf("declare %s as (%s)\n", cdecl_identifier(c), output);
		} else if (retcode > 0) {
			printf("%s\n", output);
		}

		printf(">");
	}
}
