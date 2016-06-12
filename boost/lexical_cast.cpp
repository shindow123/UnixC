#include <boost/lexical_cast.hpp>
#include <string>
#include <stdio.h>

int main() {
  std::string a = "100abc";
  int b = 50;
  int f = 5/10; 
  try {
	int c = boost::lexical_cast<int>(a);
	b += c;
	printf("b:%d\n", b);
	std::string d = boost::lexical_cast<std::string>(b);
	printf("d:%s\n", d.c_str());
//	const char* e = (boost::lexical_cast<std::string>(f)).c_str();
//	printf("e:%s\n", e);
  } catch (std::exception &e) {
	printf("1exception! %s\n", e.what());
  }
  //catch (boost::bad_lexical_cast & e) {
  //  printf("2exception! %s\n", e.what());
  //}
  return 0;
}
