#include <iostream>
#include <vector>
#include <string.h>
#include <cstdio>
#include <sys/types.h>   
#include <sys/wait.h>    
#include <signal.h>     
#include <unistd.h> 
using namespace std;

static void zom(int)
{
    int a;
    while (waitpid(-1, &a, WNOHANG) > 0) {}
}
int main() {
    char cmd[100]; 
    memset(cmd, 0, sizeof(cmd));
    int count=0;
    bool flag=0;
    string p;
    char *str;
    while(1){
        signal(SIGCHLD,zom);
        flag=0;
        count = 0;
        while (waitpid(-1, &count, WNOHANG) > 0) {}
        cout << ">";
        
        if (!getline(cin, p))
        break;
        for(char i:p){
            cmd[count] = i;
            count++;
        }
        cmd[count] = '\0';
        str=strtok(cmd," ");
        char *arg[100];
        arg[0] = str;
        count = 1;
        while(str!=NULL){
            str = strtok(NULL," ");
            arg[count] = str;
            count++;
        }
        if(count-2<0)
        continue;
        if(strcmp(arg[count-2],"&")==0)
        {
            arg[count-2]=NULL;
            flag=1;
        }
        while (waitpid(-1, &count, WNOHANG) > 0) {}
        pid_t pid;
        
        pid=fork();

        if (pid < 0) { 
            fprintf(stderr, "Fork Failed");
            exit(-1);
        }
        else if (pid == 0) { 
            execvp(arg[0],arg);
            exit(127);
        }
        else { 
        if(flag==0){
            waitpid(pid,&count,0);
        }
        else {
            continue;
        }
        }
    }
    
}