/// =========================================================================
/// Written by pfederl@ucalgary.ca in 2020, for CPSC457.
/// =========================================================================
/// You need to edit this file.
///
/// You can delete all contents of this file and start from scratch if
/// you wish, but you need to implement the getDirStats() function as
/// defined in "getDirStats.h".

#include "getDirStats.h"
#include "digester.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <utility>

static bool
is_dir( const std::string & path)
{
  struct stat buff;
  if( 0 != stat( path.c_str(), & buff))
    return false;
  return S_ISDIR(buff.st_mode);
}

// ======================================================================
// You need to re-implement this function !!!!
// ======================================================================
//
// getDirStats() computes stats about directory dir_name
// if successful, it return true and stores the results in 'res'
// on failure, it returns false, and res is in undefined state
//
//template<typename K, typename V>


bool
getDirStats(const std::string & dir_name, Results & res)
{
  // The results below are all hard-coded, to show you all the fields
  // you need to calculate. You should delete all code below and
  // replace it with your own code.
  res.all_files_size=0;
  res.n_dirs=0;
  res.n_files=0;
  res.largest_file_size=-1;
  
  if( ! is_dir(dir_name)){
    return false;
  }
  std::vector<std::string> stack;
  stack.push_back(dir_name);

  
  std::unordered_map<std::string,int> filetype_pair;
  std::unordered_map<std::string,std::vector<std::string>> hashed_files_pair;
  

  struct stat buff;
  
  char fileType[200];
  
  while( ! stack.empty()) {
    auto dirname = stack.back();
    stack.pop_back();
    

    
    DIR * dir = opendir( dirname.c_str());
    if (dir == NULL) {
      //if error return false
      return false;
    }
    if( dir) {
      if (is_dir(dirname)){  
          std::string command="file -b "+dirname+"/{.,}*";
          FILE* fp = popen(command.c_str(), "r");
          //get file types after popening
          while (fgets(fileType, 200, fp) != NULL){           
            std::string type (fileType);
            //if command tries to find files and failse since there are no files
            if ((type.find("(No such file or directory)")!=std::string::npos)) continue;
            //if there are files
            std::size_t pos= type.find_first_of(",\n");
            if (pos!=std::string::npos) type.resize(pos);
            filetype_pair[type]++; 
          }
          pclose(fp);
      }
      while(1) {
        dirent * de = readdir( dir);
        
        if( de == NULL) {
          //if no files or done traversing all files
          break;
        }
        std::string name = de-> d_name;
        if( name == "." || name == "..") continue;
        std::string path = dirname +"/"+ de-> d_name;
        
          
        if (! is_dir(path)){
          //if it is a file
          std::string hashedFile = sha256_from_file(path);/////////////////////////
          std::string hash = hashedFile.substr(0,64);
          hashed_files_pair[hash].push_back(path);
          res.n_files++;
          //get file's stats
          if (stat(path.c_str(), & buff) !=0) return false;
          res.all_files_size+=buff.st_size;
          //get the largest file size and file path
          if (res.largest_file_size < buff.st_size){
            res.largest_file_size=buff.st_size;
            res.largest_file_path=path;
          }
          
        }else{
          //if it is a directory
          res.n_dirs++;
          stack.push_back( path);
        }
                
      }
      closedir( dir);
    }
  }
  

  
  std::vector<std::pair<int,std::string>> type_pairs;
    //add types and values to the vector and exclude the directory file type
    for (auto & x : filetype_pair){
      if (x.first!="directory") type_pairs.emplace_back(-x.second , x.first);
    }
    //if there are more than 5 values, sort till we get the first 5 values
    if (type_pairs.size()> size_t(5)){
      std::partial_sort(type_pairs.begin(), type_pairs.begin()+5, type_pairs.end());
    }else{
      //if less than or equal to 5, sort it completely
      std::sort(type_pairs.begin(), type_pairs.end());
    }

  std::vector<std::pair<int,std::vector<std::string>>> dup_pairs;
  //add size of the vector and the vector from the map
   for (auto & x : hashed_files_pair){
    long sz = (long) x.second.size();
    if (sz>1) dup_pairs.emplace_back(-sz ,x.second );
  }
  //if there are more than 5 values, sort till we get the first 5 values
  if (dup_pairs.size()> size_t(5)){
    std::partial_sort(dup_pairs.begin(), dup_pairs.begin()+5, dup_pairs.end());
  }else{
    //if less than or equal to 5, sort it completely
    std::sort(dup_pairs.begin(),dup_pairs.end());
  }
  
  //Add values form both the vectors to the Result struct
  int num=0;
  for (auto& pair:type_pairs){
    if (num>=5) break;
    res.most_common_types.push_back(pair.second);
    num++;
  }
  int num2=0;
  for (auto& pair2: dup_pairs){
    if (num2>=5) break;
    res.duplicate_files.push_back(pair2.second);
    num2++;
  }
  
  
  return true;
}
