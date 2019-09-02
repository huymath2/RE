I use **IDA tools** with **Remote Linux Debuger** for this chal. 
For easier i will call our input string is **OurStr**. Deal with it? 
*********************************
I dropped ELF file to IDA to disassemble it. 
Look at label **loc_40079F**, we can see a **strlen** function for input string length checking. After call it, they compare result to 27h. Thus our string must have 27h character.
![](https://raw.githubusercontent.com/huymath2/RE/master/TokyoWesternsCTF/2019/1.png)
Next we have :
![](https://raw.githubusercontent.com/huymath2/RE/master/TokyoWesternsCTF/2019/image/1.png)
***
they take first 6 six characters of OurStr compare with string "TWCTF{" and last char with "}"(7Dh). Easily, we have flag format TWCTF{"something..."}
Now we must find 32 characters remaining. In my opiion, this is not warm up challenge :v.

As you see, this is graph overview 
![](https://raw.githubusercontent.com/huymath2/RE/master/TokyoWesternsCTF/2019/image/2.png)
***
It looks like terrible, but don't worry. if you debug carefully, everything will be easy.

_I think i should use source code for easier from now_.

We can see first check loop:
![](https://raw.githubusercontent.com/huymath2/RE/master/TokyoWesternsCTF/2019/image/3.png)
***
they use **strchr** for check how many character **0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f** we had in flag.
Then compare with this:
![](https://raw.githubusercontent.com/huymath2/RE/master/TokyoWesternsCTF/2019/image/4.png)
Therefore we can know that flag is a hex string include:
**(val : amount)** : 
  30h : 3,
  31h : 2,
  32h : 2,
  33h : 0,
  34h : 3,
  35h : 2,
  36h : 1,
  37h : 3,
  38h : 3,
  39h : 1,
  61h : 1,
  62h : 3,
  63h : 1,
  64h : 2,
  65h : 2,
  66h : 3
  
Next, we have two loops
![](https://raw.githubusercontent.com/huymath2/RE/master/TokyoWesternsCTF/2019/image/5.png)
![](https://raw.githubusercontent.com/huymath2/RE/master/TokyoWesternsCTF/2019/image/6.png)
***

These two loops simple calculate sum and xor of each four identified index. Then compare with given const value
This is context of two loop:
**First Loop**
**Sum of index**    
6, 7, 8, 9     = 15Eh
10, 11, 12, 13 = 0DAh
14, 15, 16, 17 = 12Fh
	                18, 19, 20, 21 = 131h
	                22, 23, 24, 25 = 100h
	                26, 27, 28, 29 = 131h
	                30, 31, 32, 33 = 0FBh
	                34, 35, 36, 37 = 102h

**XOR of index**  
6, 7, 8, 9     = 52h
	    10, 11, 12, 13 = 0Ch
	    14, 15, 16, 17 = 01h
	    18, 19, 20, 21 = 0Fh
	    22, 23, 24, 25 = 5Ch
	    26, 27, 28, 29 = 05h
	    30, 31, 32, 33 = 53h
	    34, 35, 36, 37 = 58h

**The Second Loop**
**Sum of index** 
6, 14, 22, 30  = 129h
	    7, 15, 23, 31  = 103h
	    8, 16, 24, 32  = 12Bh
	    9, 17, 25, 33  = 131h
	    10, 18, 26, 34 = 135h
	    11, 19, 27, 35 = 10Bh
	    12, 20, 28, 36 = 0FFh
	    13, 21, 29, 37 = 0FFh

**XOR of index**
6, 14, 22, 30  = 01h 
	    7, 15, 23, 31  = 57h
	    8, 16, 24, 32  = 07h
	    9, 17, 25, 33  = 0Dh
	    10, 18, 26, 34 = 0Dh
	    11, 19, 27, 35 = 53h
	    12, 20, 28, 36 = 51h
	    13, 21, 29, 37 = 51h

Next, we have a checking loop:
![](https://raw.githubusercontent.com/huymath2/RE/master/TokyoWesternsCTF/2019/image/6.5.png)
***
This loop check whether current index was **char** or **int**.
Thus i have true format of flag:
**c : char**
**i : integer**
**TWCTF{cciciiiiciicciiciicicciiiiciiici}**

Next, we have finnal loop and a _if_ condition:
![](https://raw.githubusercontent.com/huymath2/RE/master/TokyoWesternsCTF/2019/image/7.png)
***
Loop will calculate sum of all even index from sixth index and compare with 0x488
**If** condition give us 6 values of 6 indexs.
After understood purpose of this program, i wrote a C++ program to solve it.
I used a recursive to fill each character. it also combine with given condition to decrease complex of our program.

```C++
#include <iostream>
#include <stdio.h>
#include <cstring>

using namespace std;

struct Root{
    string context = "";
}arr[32];

int cnt[500];
int max_cnt[500];
string s = "TWCTF{00011224445567778889abbbcddeefff}";

void FindResult(int i){ //recursive
    if(i == 38){ //check last index
        int sum = 0;
        for(int z = 6; z < 37; ++z){
            sum += (int) s[z];
        }
         cout << s << endl;
    }

    string tmp = arr[i].context;
    //cout << tmp << endl;
    for(int j = 0; j < tmp.length(); ++j){
        int x = tmp[j];
        cnt[x]++;
        //cout << max_cnt[x] << endl;
        if(cnt[x] <= max_cnt[x]){ // check max
            s[i] = (char)x; //assign value for index
            //all of if condition base on all condition we have discussed
            if(i == 9){
                int x1 = s[6], x2 = s[7], x3 = s[8], x4 = s[9];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0x15E && k == 0x52){
                    FindResult(i + 1);
                }
            }
            else if(i == 13){
                int x1 = s[10], x2 = s[11], x3 = s[12], x4 = s[13];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0xDA && k == 0xC){
                    FindResult(i + 1);
                }
            }
            else if(i == 17){
                int x1 = s[14], x2 = s[15], x3 = s[16], x4 = s[17];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0x12F && k == 0x01){
                    FindResult(i + 1);
                }
            }
            else if(i == 21){
                int x1 = s[18], x2 = s[19], x3 = s[20], x4 = s[21];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0x131 && k == 0xF){
                    FindResult(i + 1);
                }
            }
            else if(i == 25){
                int x1 = s[22], x2 = s[23], x3 = s[24], x4 = s[25];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0x100 && k == 0x5C){
                    FindResult(i + 1);
                }
            }
            else if(i == 29){
                int x1 = s[26], x2 = s[27], x3 = s[28], x4 = s[29];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0x131 && k == 0x5){
                    FindResult(i + 1);
                }
            }
            else if(i == 30){
                int x1 = s[6], x2 = s[14], x3 = s[22], x4 = s[30];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0x129 && k == 0x1){
                    FindResult(i + 1);
                }
            }
            else if(i == 31){
                int x1 = s[7], x2 = s[15], x3 = s[23], x4 = s[31];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0x103 && k == 0x57){
                    FindResult(i + 1);
                }
            }
            else if(i == 32){
                int x1 = s[8], x2 = s[16], x3 = s[24], x4 = s[32];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0x12B && k == 0x7){
                    FindResult(i + 1);
                }
            }
            else if(i == 33){
                int x1 = s[30], x2 = s[31], x3 = s[32], x4 = s[33];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0xFB && k == 0x53){
                    x1 = s[9]; x2 = s[17]; x3 = s[25]; x4 = s[33];
                    k = x1 ^ x2; k ^= x3; k ^= x4;
                    if(x1 + x2 + x3 + x4 == 0x131 && k == 0xD){
                        FindResult(i + 1);
                    }
                }

            }
            else if(i == 34){
                int x1 = s[10], x2 = s[18], x3 = s[26], x4 = s[34];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0x135 && k == 0xD){
                    FindResult(i + 1);
                }
            }
            else if(i == 35){
                int x1 = s[11], x2 = s[19], x3 = s[27], x4 = s[35];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0x10B && k == 0x53){
                    FindResult(i + 1);
                }
            }
            else if(i == 36){
                int x1 = s[12], x2 = s[20], x3 = s[28], x4 = s[36];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0xFF && k == 0x51){
                    FindResult(i + 1);
                }
            }
            else if(i == 37){
                int x1 = s[34], x2 = s[35], x3 = s[36], x4 = s[37];
                int k = x1 ^ x2; k ^= x3; k ^= x4;
                if(x1 + x2 + x3 + x4 == 0x102 && k == 0x58){
                    int x1 = s[13]; x2 = s[21]; x3 = s[29]; x4 = s[37];
                    int k = x1 ^ x2; k ^= x3; k ^= x4;
                    if(x1 + x2 + x3 + x4 == 0xFF && k == 0x51){
                        FindResult(i + 1);
                    }
                }

            }
            else FindResult(i + 1);
        }
        cnt[x]--;
    }
}

int main(){
    memset(cnt, 0, sizeof cnt);
    int num[14] = {8, 10, 13, 16, 19, 20, 22, 25, 28, 29, 30, 33, 34, 35}; //all of indexs is integer
    int chara[11] = {6, 9, 14, 17, 18, 21, 24, 26, 27, 32, 36}; //all of indexs is char
    string str = "0123456789abcdef"; //all of character can be in flag
    int arr1[16] = {3, 2, 2, 0, 3, 2, 1, 3, 3, 1, 1, 3, 1, 2, 2, 3}; // max of amount
    //initialize for each index
    arr[37].context = "5";
    arr[7].context = "f";
    arr[11].context = "8";
    arr[12].context = "7";
    arr[23].context = "2";
    arr[31].context = "4";
    arr[15].context = "7";

    string chr = "abcdef";
    string integ = "012456789";
    for(int i = 0; i < 14; ++i){
        arr[num[i]].context = integ; //all of value which index can be
    }
    for(int i = 0; i < 11; ++i){
        arr[chara[i]].context = chr; //all of char which index can be
    }
    for(int i = 0; i < str.length(); ++i){
        int x = str[i];
        max_cnt[x] = arr1[i]; //max amount
    }
    FindResult(6);
}

```
And here we go. we got flag
![](https://raw.githubusercontent.com/huymath2/RE/master/TokyoWesternsCTF/2019/image/flag.png)









