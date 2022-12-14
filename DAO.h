#ifndef DAO_h
#define DAO_h
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <optional>
using namespace std;

/**
 * DAO template class used as a Persistence API to a simple database implemented
 * with a file text for each class that has to be persisted. It is an abstract
 * because some of the private helper functions must be implemented by a subclass 
 * specific to a type.
 */
template <class T>
class DAO {
	public:
		DAO(string fileURL) {
			this->fileURL = fileURL;
			ifstream f(fileURL);
			if(!f.good()) throw 11;
			f.close();
		}

		/**
		 * Returns pointer to object with given id if it exists.
		*/
		optional<T*> find(int id) {
			optional<T*> object;
			string line;
			ifstream file(fileURL);
			while(getline(file, line)) {
				if (checkLine(id, line))
					object = readObject(line);
			}
			file.close();
			return object;
		}

		/**
		 * Returns vector of pointers to all objects in database.
		*/
		vector<T*> findAll() {
			optional<T*> object;
			vector<T*> objects;
			string line;
			ifstream file(fileURL);
			while(getline(file, line)) {
				object = readObject(line);
				if (object.has_value())
					objects.push_back(object.value());
			}
			file.close();
			return objects;
		}

		/**
		 * Removes the line associated with an object (solely based on the id).
		*/
		void deletion(int id) {
			optional<string> resultLine = dataLine(id);
			if (!resultLine.has_value()) throw 12;
			removeLine(resultLine.value());
		}

		/**
		 * Updates the state of the given object in the database.
		 * If the object doesn't exist, it throws 13.
		 * */
		void update(T object) {
			int id = object.getId();
			optional<string> resultLine = dataLine(id);
			if (!resultLine.has_value()) throw 13;
			removeLine(resultLine.value());
			writeObject(object);
		}

		/**
		 * Creates object in the database by writing the corresponding line.
		*/
		void create (T object) {
			writeObject(object);
		}

		/**
		 * Returns the maximum id in the database. It is an utility to assign new ids later from Controller.
		*/
		int maxId() {
			string line;
			int max = 0;
			ifstream file(fileURL);
			regex r("(\\d+)\\s.*");
			smatch m;
			while(getline(file, line)) {
				if(regex_search(line, m, r))
					if(stoi(m[1]) > max)
						max = stoi(m[1]);
			}
			file.close();
			return max;
		}

		/**
		 * Returns wether an object exists in the database with given id.
		*/
		bool idExists(int id) {
			string line;
			ifstream file(fileURL);
			regex r("^" + to_string(id) + "\\s.*");
			while(getline(file, line)) {
				if(regex_search(line, r))
					return true;
			}
			file.close();
			return false;
		}

	protected:
        string fileURL;
        
		/**
		 * Reads object from line in database. Needs to be implemented by specific subclass.
		*/
        virtual optional<T*> readObject(string line) = 0;
        
		/**
		 * Returns line beginning by id.
		*/
        optional<string> dataLine(int id) {
        	optional<string> dataLine;
        	string line;
        	ifstream file(fileURL);
        	while(getline(file, line))
        		if (checkLine(id, line))
        			dataLine = line;
			file.close();
        	return dataLine;
        }

		/**
		 * Checks if line corresponds to given id.
		*/
        bool checkLine(int id, string line) {
        	regex r("^" + to_string(id) + "\\s.*");
        	return regex_search(line, r);
        }  

		/**
		 * Removes line from file.
		*/
        void removeLine(string dataLine) {
			ifstream file(fileURL);
		    ofstream temp;
		    temp.open("temp8647509834758.txt");
		    string line;
		    while (getline(file,line))
		        if (line != dataLine)
		            temp << line << endl;
		    temp.close();
		    file.close();
		    remove(fileURL.c_str());
		    rename("temp8647509834758.txt", fileURL.c_str());
		}

		/**
		 * Writes line to file. Line is dictated by object toString() method.
		*/
		void writeObject(T object) {
			ofstream file(fileURL, ios::app);
			file << object.toString() << endl;
			file.close();
		}
};
#endif