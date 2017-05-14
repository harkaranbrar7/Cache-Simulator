//
//  main.cpp
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            Memory Hierarchy Simulation
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Created by Harkaranjeet Singh on 5/8/17.
//  Copyright © 2017 Harkaranjeet Singh. All rights reserved.
//
// This program Consist of decribed function to simulate cache
// Cache is 2way-set-associative, total cache data size is 16 words, one word per block
// defined struct mcache to set a cache
// each cache block has 3 fields, i.e., valid bit, tag and data;
// Main memory size is 128 words in array named Memory_array[128]
// CPU register are defined as strig array register_file[8]
// it gives output of HIT and MISS
// It also gives a result of registers,cache and Memory


#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <bitset>
using namespace std;


// cache struct
struct mcache
{
    bool validbit = false;
    bool history = false;
    int tag = 0;
    string data = "   ";
};

//memory address array
string Memory_array[128];

// cache 2d array
// 8 set contains two block each
mcache mycache[8][2];

// 8 CPU Register
string register_file[8];

//  Read the data from file and stores it in array
void readMemory();

//function for decode instruction
void Decode(string data, int counter);

// function for decoding store word
void Decode2(string data,unsigned long & opcode,unsigned long & rs_register, unsigned long & rt_register, unsigned long & offset);

// function for load word data
void loadWord(int offset, int rs_register,int rt_register, string data);

//store word function
void storeWord(int rt_register,int rs_register);

// calculating the setnumber tag etc
void calculations(int offset,int rs_reg, int & SetNum, int & tag);

void searching_cache(int SetNumber, int Tag ,int rt_register, string data);

// loading to cache
void Load_to_cache(int SetNumber, int Tag,int blockNum,int register_number, string data);

//where the program start
int main()
{
    readMemory();
    return 0;
}

//  Read the data from file and stores it in array
void readMemory()
{
    string mdata;
    int loop = 0;                            //short for loop for input
    string line;                            //this will contain the data read from the file
    ifstream myfile("input.txt");        //opening the file.
    if (myfile.is_open())                //if the file is open
    {
        while (!myfile.eof())                      //while the end of file is NOT reached
        {
            getline(myfile, line);                //get one line from the file
            Memory_array[loop] = line;
            Decode(Memory_array[loop],loop);
            loop++;
        }
        
        myfile.close();                                    //closing the file
    }
    else
        cout << "Unable to open file ";                  //if the file is not open output
    
}


// decode the instruction

void Decode(string data, int counter)
{
    
    string opcode_temp, rs_reg_temp, rt_reg_temp, offset_temp;
    cout << endl
    << endl
    << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << "                                  START READING FROM MEMORY "<< counter<<"                             " << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl<<endl;
    cout << "The size of data " << data.length() << endl<< endl;
    
    // if string length is 0 or exceed then exit the code
    if(data.length() == 0 ){
        cout << "NOTHING IN MEMORY ?" <<endl<<endl;
        exit(0);
    }else if (data.length() >33  ){
        
        cout << "ERROR: EXCEED THE STRING LIMIT OF 32" <<endl<<endl;
        exit(0);
    }else
    {
        //decoding the string to opcode, rs register ,rt register and offset respectively
        
        for (int count = 0; count < 6; count++)
        {
            opcode_temp += data.at(count);
        }
        for (int count = 6; count < 11; count++)
        {
            rs_reg_temp += data.at(count);
        }
        for (int count = 11; count < 16; count++)
        {
            rt_reg_temp += data.at(count);
        }
        for (int count = 16; count < 32; count++)
        {
            offset_temp += data.at(count);
        }
        cout << "opcode"
        << " "
        << " rs "
        << "  "
        << " rt "
        << "     "
        << "  offset " << endl;
        cout << opcode_temp << " " << rs_reg_temp << " " << rt_reg_temp << " " << offset_temp << endl;
        

        // functions to  convert binary to decimal of Opcode, rs register ,rt register and offset respectively
        unsigned long long opcode = stoull(opcode_temp, 0, 2);
        unsigned long long rs_register = stoull(rs_reg_temp, 0, 2);
        unsigned long long offset = stoull(offset_temp, 0, 2);
        unsigned long long rt_register = stoull(rt_reg_temp, 0, 2);
        
        cout << opcode << "       " << rs_register<< "    " << rt_register << "          " << offset<< endl
        << endl;
        
        //calling the next function: load word if opcode = 35 and store word for opcode = 43
        if (opcode == 35)
        {
            loadWord(offset,rs_register,rt_register,data); // parameter of offset, rs_register, rt_register and data
        }
        else if (opcode == 43)
        {
            storeWord(rt_register, rs_register);  //parameter rt_register and rs_register
        }
    }
    
}


// Deocder function for store word
void Decode2(string data,unsigned long & opcode,unsigned long & rs_register, unsigned long & rt_register, unsigned long & offset)
{
    
    string opcode_temp, rs_reg_temp, rt_reg_temp, offset_temp;
    cout << endl
    << endl
    << endl;
        cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
        cout << "                                  DECODER 2                           " << endl;
        cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl<<endl;
    cout << "The size of data " << data.length() << endl<< endl;
    
    // if string length is 0 or exceed then exit the code
    if(data.length() == 0 ){
        cout << "NOTHING IN REGISTER" <<endl<<endl;
        exit(0);
    }else if (data.length() >33  ){
        
        cout << "ERROR: EXCEED THE STRING LIMIT OF 32" <<endl<<endl;
        exit(0);
    }else
    {
        //decoding the string to opcode, rs register ,rt register and offset respectively
        
        for (int count = 0; count < 6; count++)
        {
            opcode_temp += data.at(count);
        }
        for (int count = 6; count < 11; count++)
        {
            rs_reg_temp += data.at(count);
        }
        for (int count = 11; count < 16; count++)
        {
            rt_reg_temp += data.at(count);
        }
        for (int count = 16; count < 32; count++)
        {
            offset_temp += data.at(count);
        }
        cout << "opcode"
        << " "
        << " rs "
        << "  "
        << " rt "
        << "     "
        << "  offset " << endl;
        cout << opcode_temp << " " << rs_reg_temp << " " << rt_reg_temp << " " << offset_temp << endl;
        
        
        // functions to  convert binary to decimal of Opcode, rs register ,rt register and offset respectively
             opcode = stoull(opcode_temp, 0, 2);
        rs_register = stoull(rs_reg_temp, 0, 2);
             offset = stoull(offset_temp, 0, 2);
        rt_register = stoull(rt_reg_temp, 0, 2);
        
        cout<< opcode << "       " << rs_register<< "    " << rt_register << "          " << offset<< endl
        << endl;
    }
}



// Function for load word
void loadWord(int offset,int rs_register,int rt_register,string data)
{
    int Set_Number, Tag_Value;
    
    cout << endl
    << "########################################################################################" << endl;
    cout << "                                          LOAD WORD                                          "<< endl;
    cout << "##########################################################################################" << endl
    << endl;
    
    calculations(offset, rs_register, Set_Number, Tag_Value);
    
    cout<<"SET NUMBER   = "<<Set_Number<<endl;
    cout<<"TAG VALUE    = "<<Tag_Value<<endl;
    
    searching_cache(Set_Number,Tag_Value,rt_register, data);
    
}


//Function for Store word
void storeWord(int rt_register, int rs_register)
{
    
    unsigned long _opcode, _Rs_register, _Rt_register,_offset;
    int SetNumber , Tag , Memory_address;
    int register_Number = rt_register - 16;  // to get a CPU register number
    string CPU_data;                         // string to store value of CPU reg
    
    
    cout << endl<< "########################################################################################" << endl;
    cout << "                                           STORE WORD                                         " << endl;
    cout << "###########################################################################################" << endl
    << endl;
    
    CPU_data = register_file[register_Number];   // storing value of CPU reg to string

    cout << " CPU Register Number = " << register_Number<<" & CPU Regiter data is "<<CPU_data<< endl<< endl;
    
    Decode2(CPU_data,_opcode , _Rs_register, _Rt_register, _offset);
    
    cout<<"========================== Store Word ================================"<<endl<<endl;
    cout<<"RS register "<<_Rs_register<<"  RT register "<<_Rt_register<<" offset "<<_offset<<endl;
    
    //call the calculation function to get setnumber and tag value
    calculations(_offset, rs_register, SetNumber, Tag);
    
    cout<<"SetNumber = "<<SetNumber<<"   Tag = "<<Tag<<endl;
    
    searching_cache(SetNumber, Tag, _Rt_register, CPU_data);
    
    Memory_address = (Tag * 8) + SetNumber;
    
    Memory_array[Memory_address] = CPU_data;
    cout<<" Memory Address "<<Memory_address<<endl;
    
    
    for(int k=0; k<128;k++){
        cout<<"  Memory  ["<<k<<"] "<<Memory_array[k]<<endl;;
    }
    // send it to memory
    
}



void calculations(int offset,int rs_reg, int & SetNum, int & tag)
{
    long byteaddress, wordaddress;
    
    byteaddress = offset + rs_reg;        // Byte address
    wordaddress = byteaddress / 4;       // Word address
    
    SetNum = wordaddress % 8;          // Get the set number
    tag = wordaddress / 8;            // Get the tag value
    
}


//searching the cache
void searching_cache(int SetNumber, int Tag ,int rt_register, string data )
{
    
    int register_number = rt_register - 16;
    
            if (mycache[SetNumber][0].validbit == true)
            {
                if(mycache[SetNumber][0].tag == Tag){
                    cout<<" FOUND [HIT] "<<endl;
                    Load_to_cache(SetNumber, Tag, 0, register_number, mycache[SetNumber][0].data);
                }else{
                    cout<<" NOT FOUND [MISS]"<<endl;
                    Load_to_cache(SetNumber, Tag, 1, register_number, data);
                }
                
            }else if (mycache[SetNumber][1].validbit == true){
                
                if(mycache[SetNumber][1].tag == Tag){
                    
                    cout<<" FOUND [HIT] "<<endl;
                    Load_to_cache(SetNumber, Tag, 1, register_number, mycache[SetNumber][1].data);
                }else{
                    
                    cout<<" NOT FOUND [MISS]"<<endl;
                    Load_to_cache(SetNumber, Tag, 1, register_number, data);
                }

            }else if (mycache[SetNumber][0].validbit == false){
                cout<< "NOT FOUND [MISS]"<<endl;
                int blockNum = 0;
                Load_to_cache(SetNumber,Tag,blockNum,register_number,data);
                mycache[SetNumber][blockNum].validbit = true;
            }else if (mycache[SetNumber][1].validbit == false){
                cout<< "NOT FOUND [MISS]"<<endl;
                int blockNum = 1;
                Load_to_cache(SetNumber,Tag,blockNum,register_number,data);
                mycache[SetNumber][blockNum].validbit = true;
            
            }
    
    
}//function end


// Load the data to cache
void Load_to_cache(int SetNumber,int Tag,int blockNum,int register_number,string data)
{
    cout<<endl<<"i am loading bro"<<endl<<endl;
 
              mycache[SetNumber][blockNum].tag = Tag;
             mycache[SetNumber][blockNum].data = data;
                register_file[register_number] = mycache[SetNumber][blockNum].data;
    
    
    for(int k=0; k<8;k++){
        cout<<"  register  ["<<k<<"] "<<register_file[k]<<endl;;
    }
    
    
    
        for(int i=0; i<8; i++)    //This loops on the Set Number
        {
            for(int j=0; j<2; j++) //This loops on the Block
            {
                
                cout<<" Set ["<<i<<"]  Block ["<<j<<"]  "<<mycache[i][j].data<<endl;
            
            }
            cout<<endl;
        }
    
    

    
    
}





// Progress 60%
// Check without loop only if statements
// check the hit or miss again
// check about the History
// Implement store word






////    for(int i=0; i<8; i++)    //This loops on the Set Number
////    {
////        for(int j=0; j<2; j++) //This loops on the Block
////        {
//if (mycache[SetNumber][0].validbit == true)
//{
//    if(mycache[SetNumber][0].tag == Tag){
//        
//        cout<<" found [hit] "<<endl;
//        
//        }else{
//            
//            cout<<" not found [miss]"<<endl;
//        }
//        
//        }else{
//            
//            if (mycache[SetNumber][0].validbit == false)
//            {
//                mycache[SetNumber][0].validbit = true;
//                int blockNum = 0;
//                Load_to_cache(SetNumber,Tag,blockNum,register_number,data);
//            }
//            else if(mycache[SetNumber][1].validbit == false)
//            {
//                mycache[SetNumber][1].validbit = true;
//                int blockNum = 1;
//                Load_to_cache(SetNumber,Tag,blockNum,register_number,data);
//            }
//            
//            
//        }// else end
//        
//        
//        //   }// for loop second end
//        
//        
//        
//        //  } // for loop end
//        
//        
//        }//function end
//



