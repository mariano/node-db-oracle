#include <fstream>
#include <string>

class TNSParser{
  std::string connection_string, tns_name;
  public:
   TNSParser(std::string);
   std::string getConnectionString();
  private:
   std::string _toupper(std::string);
   void _processTNS();
   std::string _findEntry(std::string);
};
