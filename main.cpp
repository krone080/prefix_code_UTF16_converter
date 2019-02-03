#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <exception>
#include <codecvt>

using namespace std;

int main(int argc, char *argv[])
 {

 locale utf8_to_utf16(locale(),new codecvt_utf8<wchar_t>);
 char mode;
 for(;;)
  {
  cout<<"Encode or Decode, E/D?: ";
  while(!(cin>>mode)||(mode!='E'&&mode!='D'))
   cerr<<"Error\n";

  //ENCODER MODE
  if(mode=='E')
   {
   map<wchar_t,string> code={{L' ',"000"},
                          {L'о',"0010"},
                          {L'е',"0100"},
                          {L'а',"0110"},
                          {L'и',"1000"},
                          {L'н',"1100"},
                          {L'т',"1101"},
                          {L'с',"1110"},
                          {L'л',"00110"},
                          {L'р',"01010"},
                          {L'в',"01110"},
                          {L'м',"10100"},
                          {L'д',"10010"},
                          {L'к',"10011"},
                          {L'п',"11110"},
                          {L'у',"11111"},
                          {L',',"010110"},
                          {L'я',"010111"},
                          {L'ь',"011110"},
                          {L'г',"101100"},
                          {L'б',"101101"},
                          {L'ы',"101110"},
                          {L'з',"101010"},
                          {L'ч',"0011100"},
                          {L'.',"0011110"},
                          {L'й',"0011111"},
                          {L'ж',"0111110"},
                          {L'ю',"1011110"},
                          {L'ш',"1010110"},
                          {L'х',"00111010"},
                          {L'\n',"10111110"},
                          {L'ц',"10101110"},
                          {L'–',"10101111"},
                          {L';',"001110110"},
                          {L'щ',"011111100"},
                          {L'«',"011111110"},
                          {L'»',"101111110"},
                          {L'?',"101111111"},
                          {L':',"0111111010"},
                          {L'!',"0111111110"},
                          {L'*',"0111111111"},
                          {L'э',"00111011100"},
                          {L'-',"00111011110"},
                          {L'…',"01111110110"},
                          {L'ъ',"001110111010"},
                          {L'ф',"011111101110"},
                          {L'1',"011111101111"},
                          {L'8',"0011101110110"},
                          {L'0',"0011101110111"},
                          {L'3',"0011101111100"},
                          {L'(',"0011101111101"},
                          {L')',"0011101111110"},
                          {L'6',"00111011111110"},
                          {L'2',"001110111111110"},
                          {L'4',"001110111111111"}};

   string buff="",fname;
   wchar_t got_wchar;
   char formed_byte;

   wifstream infile;
   infile.imbue(utf8_to_utf16);

   ofstream outfile;

   while(1)
    {
    cout<<"Input in-file name: ";
    cin>>fname;
    infile.open(fname);
    if(infile.is_open()==0)
     {
     cerr<<"Error\n";
     continue;
     }
    break;
    }

   while(1)
    {
    cout<<"Input out-file name: ";
    cin>>fname;
    outfile.open(fname);
    if(outfile.is_open()==0)
     {
     cerr<<"Error";
     continue;
     }
    break;
    }

   unsigned len,bits_num=7,bits_wrote;
   unsigned counter=0;

   //Основное преобразование из UTF-16 в мою кодировку.
   //Если вдаваться в логику, то можно сломать мозг),
   //но всё работает безотказно при любом префиксном коде
   while(infile.read(&got_wchar,1),infile.good())
    {
    ++counter;
    cout<<'\r'<<counter;
    buff=code[got_wchar];
    len=buff.length();
    bits_wrote=0;
    for(unsigned i=0;i<len;++i)
     {
     if(bits_num-(i-bits_wrote)==0)
      {
      if(buff[i]=='1')
       formed_byte+=1<<bits_num-(i-bits_wrote);
      outfile<<formed_byte;
      formed_byte=0;
      bits_wrote=i+1;
      bits_num=7;
      continue;
      }
     if(buff[i]=='1')
      formed_byte+=1<<bits_num-(i-bits_wrote);
     if(i==len-1)
      bits_num-=(i-bits_wrote)+1;
     }
    }
   //тупо все оставшиеся биты заполняем единицами, т.к. ни одного
   //кодового слова из одних единиц не существует (кроме у)
   if(bits_num!=7)
    for(unsigned i=0;i<=bits_num;++i)
      formed_byte+=1<<bits_num-i%8;
   outfile<<formed_byte;
   cout<<'\n';
   }
  //DECODER MODE
  else
   {
   map<string,wchar_t> decode={{"000",L' '},
                               {"0010",L'о'},
                               {"0100",L'е'},
                               {"0110",L'а'},
                               {"1000",L'и'},
                               {"1100",L'н'},
                               {"1101",L'т'},
                               {"1110",L'с'},
                               {"00110",L'л'},
                               {"01010",L'р'},
                               {"01110",L'в'},
                               {"10100",L'м'},
                               {"10010",L'д'},
                               {"10011",L'к'},
                               {"11110",L'п'},
                               {"11111",L'у'},
                               {"010110",L','},
                               {"010111",L'я'},
                               {"011110",L'ь'},
                               {"101100",L'г'},
                               {"101101",L'б'},
                               {"101110",L'ы'},
                               {"101010",L'з'},
                               {"0011100",L'ч'},
                               {"0011110",L'.'},
                               {"0011111",L'й'},
                               {"0111110",L'ж'},
                               {"1011110",L'ю'},
                               {"1010110",L'ш'},
                               {"00111010",L'х'},
                               {"10111110",L'\n'},
                               {"10101110",L'ц'},
                               {"10101111",L'–'},
                               {"001110110",L';'},
                               {"011111100",L'щ'},
                               {"011111110",L'«'},
                               {"101111110",L'»'},
                               {"101111111",L'?'},
                               {"0111111010",L':'},
                               {"0111111110",L'!'},
                               {"0111111111",L'*'},
                               {"00111011100",L'э'},
                               {"00111011110",L'-'},
                               {"01111110110",L'…'},
                               {"001110111010",L'ъ'},
                               {"011111101110",L'ф'},
                               {"011111101111",L'1'},
                               {"0011101110110",L'8'},
                               {"0011101110111",L'0'},
                               {"0011101111100",L'3'},
                               {"0011101111101",L'('},
                               {"0011101111110",L')'},
                               {"00111011111110",L'6'},
                               {"001110111111110",L'2'},
                               {"001110111111111",L'4'}};

   string buff="",fname;
   wchar_t got_wchar;
   char formed_byte;

   ifstream infile;

   wofstream outfile;
   outfile.imbue(utf8_to_utf16);

   while(1)
    {
    cout<<"Input in-file name: ";
    cin>>fname;
    infile.open(fname);
    if(infile.is_open()==0)
     {
     cerr<<"Error\n";
     continue;
     }
    break;
    }

   while(1)
    {
    cout<<"Input out-file name: ";
    cin>>fname;
    outfile.open(fname);
    if(outfile.is_open()==0)
     {
     cerr<<"Error";
     continue;
     }
    break;
    }

   unsigned counter=0;
   //Основное преобразование из файла моей кодировки в файл
   //кодировки UTF-16. Здесь попроще.
   while(infile.read(&formed_byte,1),infile.good())
    {
    ++counter;
    cout<<'\r'<<counter;

    for(int i=7;i>=0;--i)
     {
     if((formed_byte&(1<<i))>>i==1)
      buff.push_back('1');
     else
      buff.push_back('0');
     if(decode.count(buff)==1)
      {
      outfile<<decode[buff];
      buff.clear();
      }
     }
    }
   cout<<'\n';
   }
  }
 return 0;
 }
