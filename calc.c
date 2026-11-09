#include <stdio.h>
#include <ctype.h>
#include <math.h>

enum TYPE{INT, PLUS, MINUS, MUL, DIV, POW, LBRACKET, RBRACKET, END, INVALID};

typedef struct 
{
   int type;
   int value;
} Token;

typedef struct 
{
    char *input;
    int pos;
} Lexer;

int errorFlag;
char current_char;
Token current_token;
Lexer global_lexer;
int MidExpr();
void Consume(int p);
int UpperExpr();
int Func();

int peek(){
    int nextpos = global_lexer.pos;   
    char nextchar = global_lexer.input[nextpos];
    if (nextchar == '(')
        return LBRACKET;
    // else if (nextchar == 0)
    //     return 0;
}

//multiple digit number handling
int GetNumber(){
    int res = 0;
    while(isdigit(current_char) && current_char != 0){
        res = res*10 + (current_char - '0');
        global_lexer.pos++;
        current_char = global_lexer.input[global_lexer.pos] ;
    }      
    return res;
}

void SendWithMul(int mul){
    int num = mul;
    num*=Func();
}

//Lexer

Token LexerNext(Lexer *lexer){ //returns token of current value and inc pos
    char *text = lexer->input;
    int pos = lexer->pos;
    current_char = text[pos];

    if(current_char == 0){
        Token token = {
            .type = END,
            .value = 0
        };
        lexer->pos++;
        return token;
    }

    else if(isspace(current_char)){
        lexer->pos++;
        return LexerNext(lexer);
    }

    else if(isdigit(current_char)){
        Token token = {
            .type = INT,
            .value = GetNumber()
        };
        //not inc lexer pos becauze i already did in GetNumber
        // if (current_char == LBRACKET){
        //    SendWithMul(token.value);
        // }
        return token;
    }

    else if (current_char == '+'){       
        Token token = {
            .type = PLUS,
            .value = 0
        };
        lexer->pos++;
        return token;
    }
    
     else if (current_char == '-'){       
        Token token = {
            .type = MINUS,
            .value = 0
        };
        lexer->pos++;
        return token;
    }

     else if (current_char == '*'){       
        Token token = {
            .type = MUL,
            .value = 0
        };
        lexer->pos++;
        return token;
    }
     else if (current_char == '/'){       
        Token token = {
            .type = DIV,
            .value = 0
        };
        lexer->pos++;
        return token;
    }
    
    else if (current_char == '^'){       
        Token token = {
            .type = POW,
            .value = 0
        };
        lexer->pos++;
        return token;
    }

     else if (current_char == '('){       
        Token token = {
            .type = LBRACKET,
            .value = 0
        };
        lexer->pos++;
        return token;
    }

     else if (current_char == ')'){       
        Token token = {
            .type = RBRACKET,
            .value = 0
        };
        lexer->pos++;
        return token;
    }
    
    else{
        Token token = {
            .type = INVALID,
            .value = 0
        }; 
        
        return token;
    }
    

}

//Parser
int LowerExpr(){ //+ -
    int num = MidExpr();

    while(current_token.type == PLUS || current_token.type == MINUS)
    {
        if(current_token.type == PLUS){
            Consume(PLUS);
            num+=MidExpr();
        }
        else{
            Consume(MINUS);
            num-=MidExpr();
        }
       
    } return num;
}
int MidExpr(){ //* /
    int num = UpperExpr();

    while(current_token.type == MUL || current_token.type == DIV || current_token.type == LBRACKET)
    {
        if(current_token.type == MUL){
            Consume(MUL);
            num*=UpperExpr();
        }
        else if (current_token.type == DIV){
            Consume(DIV);
            int div = UpperExpr();
            if(div == 0){
                printf("cant devide by 0\n");
                errorFlag = 1;
                return 0;
            }           
            num/=div;
        }
        else{
            num*=UpperExpr();
        }
     
    }   return num;
}
int UpperExpr(){ //^
    int num = Func();

    if(current_token.type == POW){
        Consume(POW);
        num = (int)pow(num, Func());
    }
    return num;
}

int Func(){ //int or parant
    if(current_token.type == INT){
        int val = current_token.value;
        Consume(INT);                  
        return val;
    }
    else if(current_token.type == MINUS){       
        Consume(MINUS);                  
        return -Func();
    } 
    else if(current_token.type == LBRACKET)
    {
        Consume(LBRACKET);
        int res =  LowerExpr();
        Consume(RBRACKET);
        return res;
    }
}


int Func2(){ //int or parant
    
    if(current_token.type == LBRACKET)
    {
        Consume(LBRACKET);

        int res =  LowerExpr();
        Consume(RBRACKET);
        return res;
    }
}

// int Calculate(){
//     int num = Func();


// }

void Consume(int type){ //expected input, if met advance token, if ) expected, write closing expected, else error
    
    if(type == current_token.type)
        current_token = LexerNext(&global_lexer);
    
    else{
        if( type == RBRACKET)
            printf("Expecting ')'");
        else{
            printf(
                "expected %d, got %d", type, current_token.type
            );
        }
    }
}
int main(){
    char input[512];
    
    while(1){
        printf("> ");

        if(!fgets(input, sizeof(input), stdin))
            break;
        
        Lexer lexer = {
            .input = input,
            .pos = 0
        };
        global_lexer = lexer;
        current_token = LexerNext(&global_lexer);
        
        // while (1) {
        // Token token = LexerNext(&lexer);
        // if (token.type == END || token.type == INVALID)
        //     break;
        // printf("int of enum: %d, value: %d\n",
        //    token.type, token.value);
        // }
        errorFlag = 0;
        int res = LowerExpr();
        if(!errorFlag)
            printf("%d\n", res );
    }
    return 0;
}