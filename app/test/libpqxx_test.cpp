#include <iostream>
#include <pqxx/pqxx>

int main()
{
  try
  {
    pqxx::connection c;
    std::cout << "Connected to " << c.dbname() << '\n';
  } catch (std::exception const& e) {
    std::cerr << "ERROR: " << e.what() << '\n';
    return 1;
  }
  return 0;
}
