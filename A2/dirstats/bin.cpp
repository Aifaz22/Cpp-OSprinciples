// gives correct result 
        // std::string command="file -b "+path;//+"*";
          // FILE* fp = popen(command.c_str(), "r");
          // //printf("%s\n", dirname.c_str());
          // while (fgets(fileType, 200, fp) != NULL){
          //   std::string type (fileType);
          //   std::size_t pos= type.find_first_of(",\n");
          //   if (pos!=std::string::npos)  type.resize(pos);
          //   filetype_pair[type]++;
            
          // }
          // pclose(fp);




          //unused_hash++;
          

          // std::string command="file -b "+path;//+"*";
          // FILE* fp = popen(command.c_str(), "r");
          // //printf("%s\n", dirname.c_str());
          // while (fgets(fileType, 200, fp) != NULL){
          //   std::string type (fileType);
          //   std::size_t pos= type.find_first_of(",\n");
          //   if (pos!=std::string::npos)  type.resize(pos);
          //   filetype_pair[type]++;
            
          // }
          // pclose(fp);
          


          //GIVES SLIGHTLY INCORRECT RESULT but significantly fast***************************************************
          // std::string command="file -b "+path+"/*";
          // FILE* fp = popen(command.c_str(), "r");
          
          // while (fgets(fileType, 200, fp) != NULL){
          //   std::string type (fileType);
          //   std::size_t pos= type.find_first_of(",\n");
          //   if (pos!=std::string::npos)  type.resize(pos);
          //   if (type.find("cannot open")!=std::string::npos) type="empty";
          //   filetype_pair[type]++; 
          // }
          // pclose(fp);
          
           //path.push_back('/');