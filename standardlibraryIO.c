#include<stdio.h>
#include<string.h>

int main()
{
    FILE* fp = fopen("./test.txt", "w+"); //"w+"表示以读写方式打开,文件不存在时则创建
    if(fp == NULL)
    {
        perror("fopen error");
        return -1;
    }

    //fseek跳转读写位置 
    //SEEK_SET --从文件开头偏移 SEEK_CUR --从当前读写位置偏移 SEEK_END --从文件末尾偏移
    fseek(fp, 0, SEEK_END); //将文件的读写位置偏移到末尾
    
    char buf[] = "Hello World!";
    int ret = fwrite(buf, strlen(buf), 1, fp);
    if(ret == 0)
    {
        perror("fwrite error");
        return -1;
    }
   
    fseek(fp, 0, SEEK_SET); //将文件的读写位置偏移到开头
   
    char tmp[1024] = {0};
    ret = fread(tmp, 1, 1023, fp);  //在块大小为1的情况下不会出现读到数据依然返回0的情况
    if(ret == 0)
    {
        printf("have no data or error\n");
    }
    printf("ret:%d-[%s]\n", ret, tmp);
   
    fclose(fp);
    return 0;
} 
