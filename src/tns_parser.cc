#include "./tns_parser.h"


TNSParser::TNSParser(std::string tns_name){
  this->tns_name = this->_toupper(tns_name);
  this->connection_string = "";
  this->_processTNS();
};



std::string TNSParser::getConnectionString(){
  return this->connection_string;
};


std::string TNSParser::_toupper(std::string parser){
   std::string return_val = "";
   for(int i=0; i < parser.length(); i++){
     return_val += toupper(parser.at(i));
   }
   return return_val;
};

void TNSParser::_processTNS(){
  char *nTNS, *nOCL;
  nTNS = getenv("TNS_ADMIN");
  nOCL = getenv("ORACLE_HOME");
  if(nTNS == NULL && nOCL == NULL){
   // Through Variable Undefined Error
  }else{
    std::string tns_entry;
    if( nTNS != NULL ){
      tns_entry = this->_findEntry( strcat(nTNS, "tnsnames.ora" )  );
    } 
    if( nOCL != NULL && tns_entry.c_str() == NULL){ 
      tns_entry = this->_findEntry( strcat(nOCL, "/network/admin/tnsnames.ora") );
    }
    if( tns_entry.c_str() != NULL){
      this->connection_string = tns_entry;
    }
   }
}

std::string TNSParser::_findEntry(std::string file_path){
   std::string line;
   bool spool = false;
   bool first = true;
   std::string tns_entry;
   int pos;
   int count = 0;
   std::fstream tns_file (file_path.c_str(), std::fstream::in);
   if( tns_file.is_open() ){
     while( tns_file.good() ){
       getline(tns_file, line);
       pos = 0;
       if(!spool){
         if((pos = this->_toupper(line).find( this->tns_name )) != std::string::npos){
           pos = line.find('=', pos+1 ) + 1;
           spool = true;
         }
       }
       if( spool ){
         while(true){
           if( pos >= line.length() ){
             break;
           }else{
             char letter = line.at(pos);
             if(letter == '('){
               count += 1;
             }
             if(letter == ')'){
               count -= 1;
               if(count == 0){ 
                 tns_entry += letter;
                 spool =false;
                 break;
               }
             }
             tns_entry += letter;
           }
           pos += 1;
         }
         
        
       }
     }
     tns_file.close();
   }else{
    // Through File Missing error
   }
   return tns_entry;

} 
