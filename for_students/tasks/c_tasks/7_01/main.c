#include <unistd.h>
#include <err.h>

int main ()
{
		execl("/bin/date", "date", "-R", NULL); 
		err(99, "err execling");
}
