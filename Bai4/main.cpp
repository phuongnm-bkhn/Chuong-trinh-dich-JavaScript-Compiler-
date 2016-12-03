#include <stdio.h>

int main_ ()
{
    printf("ahihi");
    getchar();
    return 1;
}

int main(int argc, char **argv)
{
   
	vector<TOKEN_ID> lstToken = getListToken("data/code.js");

    for (int i = 0; i < lstToken.size(); i++)
    {
	TOKEN_ID id = lstToken[i];
	printf("\ntoken => %s", g_mapTokenName[id].c_str());
    }
    getchar();
}
