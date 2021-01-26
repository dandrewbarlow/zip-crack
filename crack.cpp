// Andrew Barlow
// a c++ zip encryption cracker
// this one's just 4 fun

// LIBRARIES/////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <zip.h>
#include <getopt.h>

using namespace std;


// GLOBAL VARS///////////////////////////////////////////////////////////

string characters = "abcdefghijklmnopqrstuvwxyz";
vector<string> permutations;

string zipFile = "secret.txt.zip";
string targetFile = "secret.txt";
int length = 4;


// OPTIONS///////////////////////////////////////////////////////////////
// -s : silent
// -k : k length (of calculated permutations; exponential computation)
// -l : list of passwords to check
// -f : zip file
// -t : target file (inside zip)
// -o : output file

/*
void parseOptions(int argCount, char * arguments[]) {
    int flags, opt;
    int nsecs, tfnd;

    while ((opt = getopt(argc, argv, "l:f:")) != -1) {
        switch (opt) {
        case 'l':
            length = atoi(optarg);
            break;
        case 'f':
            nsecs = atoi(optarg);
            tfnd = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-l length] [-f] filepath\n",
                    argv[0]);
            exit(EXIT_FAILURE);
    }
    if (argCount > 0) {
        for (int i = 1; i < argCount; i++) {
            if ( strncmp(arguments[i], "-f", 2) == 0) {
                encryptedZip = arguments[i+1];
            }
            else if ( strncmp(arguments[i], "-l", 2) == 0) {
                length = (int) arguments[i+1];
            }
        }
    }
}


*/
// credit to https://www.geeksforgeeks.org/print-all-combinations-of-given-length/
// modified, but only a lil
// Gets all permutations of K length of a list w/ length n; prefix is for recursive implementation, empty on start
void getKPermutations(string list, string prefix, int n, int k) {

    if (k == 0) {

        permutations.push_back(prefix);
        // cout << prefix << endl;
        return;
    }
    for (int i = 0; i < n; i++) {

        string newPrefix = prefix + list[i];
        
        getKPermutations(list, newPrefix, n, k-1);
    
    }

}

void getAllPermutations(int len) {

    int n = characters.length();

    for (int i = 0; i <= len; i++ ) {
    
        getKPermutations(characters, "", n, i);
    
    }
}

int readOutFile(zip_t * archive, zip_file_t * file, struct zip_stat stat) {
    //Allocate memory for its uncompressed contents
    char *contents = new char[stat.size];

    // Read the opened file to contents buffer
    zip_int64_t zipRead;
    try {
        zipRead = zip_fread(file, contents, stat.size);
    }
    catch (...) {
        // ensure zipRead has -1 value on any failure
        zipRead = -1;
    }

    if ( zipRead != -1 ) {
        zip_fclose(file);
        
        //And close the archive
        zip_close(archive);
        
        try {
            ofstream(targetFile).write(contents, stat.size);
            return 1;
        }
        catch (...) {
            cout << "error writing file";
        }
    }
    return 0;
}

// attempt to use password to open file and write it to outfile
int openZip(zip_t * file, struct zip_stat stat, zip_flags_t flags, string password) {
    
    zip_file_t * attempt;
    attempt = zip_fopen_encrypted( file, targetFile.c_str(), flags, password.c_str() );

    /*
    currently generating a LOT of false positives, so better solution required
    libzip maintainer maintains this is a flaw of the the file standard itself however, so not much I can do without some blue sky solutioneering
    UPDATE changed to checking for "No error" string, now I get no positives, even for the correct password. fuck this shit
    */

    // check if the password worked
    // libzip error codes:
    // ZIP_ER_OK : "No error" 
    // ZIP_ER_WRONGPASSWD : 0x7f84c0d00180 : "Wrong password provided"

    bool validPassword = zip_get_error(file) == ZIP_ER_OK;

    if (validPassword) {
        cout << password << endl;
        return readOutFile(file, attempt, stat);
    }
    else {
        // cout << zip_error_strerror( zip_get_error(file) ) << endl;
        zip_error_clear(file);
    }

    return 0;
}


int bruteForce() {
    // initialize libzip flags as readonly and init error var
    zip_flags_t flags = 0 | ZIP_RDONLY;
    int error;

    // convert the parameters to c-strings to make libzip happy
    const char * arguments[] = { zipFile.c_str(), targetFile.c_str() };

    // open the given zip w/ my flags and error val
    zip_t * encryptedZip = zip_open(arguments[0], flags, &error);

    // get the stats of the file I'm trying to extract
    // used to define buffer size
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(encryptedZip, arguments[1], 0, &st);

    // using for debugging
    int tries = 0;

    // zip_file_t * control = zip_fopen_encrypted( encryptedZip, arguments[1], flags, "pass" );
    openZip( encryptedZip, st, flags, "pass" );

    for (string password : permutations) {
        tries++;
        if ( openZip( encryptedZip, st, flags, password ) == 1 ) {
            return 1;
        }
    }
    
    cout << "Tries: " << tries << endl;
    // cout << "Matches: " << matches << endl;
    // cout << "Reads: " << reads << endl;
    // cout << "Writes: " << writes << endl;

    return 0;
}

int main(int argc, char * argv[]) {

    // parseOptions(argc, argv);

    getAllPermutations(length);
    // cout << "Permutations generated" << endl;

    return bruteForce();

    // cout << "Brute Force Protocol Complete" << endl;
}