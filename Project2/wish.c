#include <stdio.h>
#include <stdlib.h>
char *wish_read_line();

int main(int argc, char** argv) {
	wish_read_line();
	
	return 0;
}

char *wish_read_line(){
	
    unsigned int maximum_length = 64;
    unsigned int current_size = 0;

    char *string_pointer = malloc(maximum_length);
    current_size = maximum_length;
    
    if(string_pointer != NULL)
    {
        int c = EOF;
        unsigned int i =0;

        // Accept user input until hit enter or end of file
        while (( c = getchar() ) != '\n' && c != EOF)
        {
            string_pointer[i++]=(char)c;

            // If i reached maximize_size then realloc size
            if(i == current_size)
            {
                current_size = i+maximum_length;
                string_pointer = realloc(string_pointer, current_size);
            }
        }

        // Add an implicit NULL terminator
        string_pointer[i] = '\0';
    }
	
	printf("%s", string_pointer);
}


