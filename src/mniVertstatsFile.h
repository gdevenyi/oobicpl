#ifndef __MNIVERTSTATSFILE__
#define __MNIVERTSTATSFILE__

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include <tree.hh>


/*! \addtogroup vertstats Dealing with vertstats files*/
/*@{*/


using namespace std;

/*! A type to hold a vector of a vector */
typedef vector<float> vertexColumn;
typedef vector<vertexColumn> vertexMatrix;

/*! the type of file that is being read */
enum mniVertstatsFileType { OLDSTYLE, NEWSTYLE };

/*! A class that holds the header information in a key value pair 
 *
 * Each header is kept in a key value pair combination, both
 * represented as STL strings. The header can be constructed by
 * calling the constructor with two strings as arguments, or accessing
 * the public key and value members of the class. */

class mniVertstatsHeaderEntry {
public:
  mniVertstatsHeaderEntry(string a, string b) { key = a; value = b; }
  mniVertstatsHeaderEntry() { /* */}
  string key;
  string value;

  /*! comparison operator
   *
   * compares a string to the key of the headerEntry
   */
  bool operator== (string a) {
    if (a == key)
      return true;
    else
      return false;
  }
        
};

/*! \brief A class to read vertex statistics files.
 * 
 * A class designed to read and interact with vertex statistics files
 * as generated by the vertex based morphometry techniques. These
 * files contain the following elements: a header, containing the
 * formula used to generate the data, statistics performed on the
 * mean, etc., and a body containing the actual statistical elements
 * in a m*n matrix, where m is the number of vertices and n is the
 * number of statistical terms *(F stats, adjusted R square, etc.).
 *
 * A certain number of terms are important:
 * <ul>
 *  <li><b>header:</b> The header is a XML like structured set of text data
 *    that can hold any arbitrary information about the file. The header is 
 *    internally stored in a n-ary tree, and is thus fully hierarchical.
 *  <li><b>data header:</b> The data header contains a single word describing
 *    every data column.
 *  <li><b>data:</b> The data is organised as a matrix, each column
 *    representing a distinct and potentially unrelated bit of information
 *    about each vertex. Only scalars are allowed at this point.
 * </ul>
 *
 */

class mniVertstatsFile {
private:
  tree<mniVertstatsHeaderEntry> *headerTree;
  vertexMatrix *data;
  vector <string> *dataheader;
  int numColumns;
  int numRows;
  mniVertstatsFileType determineFileType(char *filename);
  void initialiseVariables();
  void destroyVariables();
  void loadOldStyleFile(char *filename, bool readData=true);
  void loadNewStyleFile(char *filename, bool readData=true);
  void writeHeaderEntry(tree<mniVertstatsHeaderEntry>::iterator it, 
                        ofstream &outFile);
  //! returns an iterator for a header
  tree<mniVertstatsHeaderEntry>::iterator getHeaderIterator(string key);

public:
  //! Empty constructor.
  mniVertstatsFile();
  //! Constructor with filename (char *)
  mniVertstatsFile(char *filename, bool readData=true);
  //! Constructor with filename (string)
  mniVertstatsFile(string filename, bool readData=true);
  //! load a filename
  void loadFile(char *filename, bool readData=true);
  //! close the currently accessed file
  void close();
  //! return a data column vector identified by name
  vertexColumn getDataColumn(string rowName);
  //! return a data column vector identified by number
  vertexColumn getDataColumn(int index=0);
  //! add a new data column to the file
  void putDataColumn(vertexColumn data, string columnName);
  //! add a new header to the file
  void putHeader(mniVertstatsHeaderEntry header, string headerParent="header");
  //! add an entire header tree
  void putHeader(tree<mniVertstatsHeaderEntry> headerTree);
  //! Adds a value to a header
  void addToHeader(string header, string value);
  //! Parses the command line and adds it to the history header
  void addToHistory(int argc, char *argv[]);
  //! Adds this string to history header
  void addToHistory(string moreHistory);
  //! return the data header vector
  vector<string> getDataHeader() { return *dataheader; }
  //! return the number of columns in this file
  int getNumColumns() { return numColumns; }
  //! return the number of rows in this file
  int getNumRows() { return numRows; }
  //! print the header structure to stdout
  void printHeaderStructure();
  //! return a string identified by the header key
  string getHeaderValue(string headerKey);
  //! write the vertstats object to file
  bool writeFile(string filename, bool oldStyle=false);
  /*! An exception class for retrieving a nonexistent row */
  class InvalidColumnError { };
  /*! An exception class when a searched for header ain't there */
  class HeaderNotFoundError { };


};

/*@}*/

#endif // __MNIVERTSTATSFILE__
