#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int op_preced(const char c)
{
    switch(c)
    {   case '^':
        return 4;

        case '*':
        case '/':
        return 3;

        case '+':
        case '-':
        return 2;

      }
    return 0;
}

float applyOperator (float left, float right, char op)
{
  switch (op)
  {
    case '^': return pow (left, right);
    case '*': return left * right;
    case '/': return left / right;
    case '+': return left + right;
    case '-': return left - right;

  default:
  printf("Unknown operator\n");
  }
  return 0;
}

#define is_operator(c) (c == '+' || c == '-' || c == '/' || c == '*'|| c == '^')
#define is_ident(c) (c >= '0' && c <= '9')

bool shunting_yard(const char *input, char *output)
{
    const char *strpos = input, *strend = input + strlen(input);
    char c, stack[32], stackcurrent, *outpos = output;
    unsigned int stacklevel = 0;
    while(strpos < strend)
    {
        c = *strpos;

        // If the token is a number (identifier), then add it to the output queue.
        if(is_ident(c) || c == ' ' || c== '.')
        {
            *outpos = c; ++outpos;
        }
        // If the token is an op1 operator, then:
        else if(is_operator(c))
        {
            while(stacklevel > 0)
            {
                stackcurrent = stack[stacklevel - 1];
                // While at the top of the stack there is a token operator op2,
                // as well as the op1 operator is left-associative and its priority is lower or the same as that of the op2 operator,
                // or the operator op1 is right-associative and its priority is less than that of operator op2
                if(is_operator(stackcurrent) && op_preced(c) <= op_preced(stackcurrent))//ckeck priority
                {
                    // Move the op2 operator from the stack to the output queue.
                    *outpos = stackcurrent; ++outpos;
                    stacklevel--;
                }
                else
                {
                    break;
                }
            }
            // put op1 into the stack
            stack[stacklevel] = c;//put operator on the top of the stack if it has higher priority
            ++stacklevel;
            *outpos = ' ';++outpos;
        }
        // If the token is a left parenthesis, then put it on the stack.
        else if(c == '(')
        {
            stack[stacklevel] = c;
            ++stacklevel;
        }
        // If the token is a right parenthesis:
        else if(c == ')')
        {
            bool parenthesesEncountered = false;
            // Until the left parenthesis appears on top of the token stack
            // move the statements from the stack to the output queue.
            while(stacklevel > 0)
            {
                stackcurrent = stack[stacklevel - 1];
                if(stackcurrent == '(')
                {
                    parenthesesEncountered = true;
                    break;
                }
                else
                {
                    *outpos = stackcurrent; ++outpos;
                    stacklevel--;
                }
            }
            // If the stack ends before the token is found, left-round parenthesis, then the bracket was skipped.
            if(!parenthesesEncountered)
            {
                printf("Error: parentheses mismatched\n");
                return false;
            }
            // throw the "left parenthesis" token from the stack (do not add to the output queue).
            stacklevel--;//ignor the parentheses
        }
        else
        {
            printf("Unknown token '%c'\n", c);
            return false; // Unknown token
        }

        ++strpos;
    }
    // When there are no tokens left on the input:
    // If tokens are left in the stack:
    while(stacklevel > 0)
    {
        stackcurrent = stack[stacklevel - 1];//take last operator from the stack
        if(stackcurrent == '(' || stackcurrent == ')')
        {
            printf("Error: parentheses mismatched\n");
            return false;
        }
        *outpos = stackcurrent; ++outpos;//put operator in queue
        --stacklevel;
    }

    *outpos = 0; // Add a trailing zero to the string.
    return true;
}

void calculate(const char *input)
{
    printf("output: %s\n", input);
    const char *strpos = input, *strend = input + strlen(input);
    char c;
    int stacklevel = 0, stackcurrent;
    float stack[32];

    while(strpos < strend)
    {
      int lange= 1;
      c = *strpos;

      if(is_ident(c))
      {
          // Put it on the stack
          //stack[stacklevel] = c - 48;// convert char to int
          //++stacklevel;
          double nummer;
          sscanf (strpos, "%lf%n", &nummer, &lange);
          stack [stacklevel] = nummer;
          ++stacklevel;
      }
      else if(is_operator(c))
      {
        float right = stack[stacklevel- 1];// last element in stack
        float left = stack [stacklevel -2];
        stacklevel = stacklevel -2;
        stack [stacklevel] = applyOperator (left, right, c);
        stacklevel++;
      }

      /*printf("Parsed character '%c', stack is now:\n", c);
      for (int i=0; i < stacklevel; i++)
        printf("%f,", stack[i]);
      printf("\n");
      */
      strpos = strpos + lange;
    }
    if (stacklevel == 1)
      printf("The result is %f", stack[0]);
    else
      printf("The error occured");
  }


int main()
{
    char input[128];
    printf("Enter the expression\n");
    scanf("%s", input );
    char output[128];
    if(shunting_yard(input, output))
    {
        printf("output: %s\n", output);
        calculate(output);
    }
    return 0;
}
