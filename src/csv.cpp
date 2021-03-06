//
// Created by bartek on 14.01.17.
//

#include <bayesian_webclass/csv.h>
#include <fstream>
#include <iostream>


/**  Parses csv file given with the filename to a map (int-string).
 *  Takes name of the csv file and numbers of the columns in the csv
 *  that will be used respectively as keys and values od the map.
 *  Keys should be type compatible with ints and values with strings.
 *  @param filename name of the csv file
 *  @param col1 csv column number holding keys for the map
 *  @param col2 csv column number holding values for the map
 *  @return boolean, true if function succeded
 */

bool Csv::csv2map(const std::string &filename,
        const int col1, const int col2) {
    // Input stream to read the csv. Each line should hold one url.
    std::ifstream url_csv(filename);
    std::string::size_type sz; //needed to stoi -> string to int
    //key -> id of record in csv, value url name

    if (url_csv.is_open()) {
        std::string line;
        boost::char_separator<char> sep{";"}; //used to tokenize csv row

        getline(url_csv, line); //in first line are identifiers, don't need them
        int max_col = (col1 > col2) ? col1 : col2;
        int counter_invalid_id = _max_invalid_ids; //number of ommited invalid ids
        while (!url_csv.eof()) {
            //tokenizing csv file line by line
            getline(url_csv, line);
            if (line == "") {
                break;
            }
            t_tokenizer tok{line, sep};

            int col_num = 0, id = 0;
            std::string domain;
            if (counter_invalid_id < 0) {
                std::cerr << "Too many invalid ids" << std::endl;
                return false;
            }

            try {
                for (auto &t : tok) {
                    if (col_num == col1) {  // Column of keys
                        id = std::stoi(t, &sz);
                    } else if (col_num == col2) {  // Column of values
                        domain = t;
                    } else if (col_num > max_col) {
                        // No need to check later columns
                        break;
                    }
                    ++col_num;
                }
            } catch (std::invalid_argument &) { //if std::stoi throws exeption, we ignore this row with invalid id
                //   std::cerr << "invalid id, counter:" << counter_invalid_id<< std::endl;
                --counter_invalid_id;
                continue;
            }

            if (col_num <= max_col) {
                //data from column_numbers and csv file doesn't match, csv hasn't enough columns
                std::cerr << "Number of columns from csv file is smaller than values wanted" << std::endl;
                //  std::cerr << "cols: " << col_num << "s: " << s << std::endl;
                return false; //TODO jakos tu rzucac wyjatek
            } else {
                _id_url_map->insert(std::pair<int, std::string>(id, domain));
            }
        }
    } else {
        std::cerr << "Cannot open file: " << filename
            << std::endl;
        return false;
    }
    url_csv.close();
    return true;
}

/**Getter
 */
const std::unique_ptr<Csv::map,
      std::default_delete<Csv::map>> &Csv::getId_url_map() const {
          return _id_url_map;
      }

