# Key-Sorting-Indexing-Tool
In this assignment, you will develop a tool which implements the key-sorting approach to create an index for a given data-file, and use them in an example application to show a search operation in action.

In JSON file, dataFileName and indexFileName are the names of data file name and index
file name, including full path, respectively. The datafile contains any type of records where records
come in no particular order. By the way, datafile must be exist and given at the beginning. Value of
recordLength shows the length of data file records in bytes.
Normally, index file is not exist initially and your tool will create it when corresponding method is
invoked. Index file includes only key and relative record numbers pointing to record in datafile, as in
key-sorting approach of simple indexing. Index file is sorted in the order of order field value, where
ASC and DESC are ascending and descending, relatively. Value of keyEncoding shows how key
field is to be interpreted: CHR for one byte long text, BIN for binary, UTF for multi-byte character
representation.
To test your application, a sample datafile shown below will be provided to you.
rowID(BIN,4)
studentID(CHR,10)
firstName(UTF,20),
lastName(UTF,20),
phone(CHR,10)

Please pay attention that, your application will be tested with another datafile and/or setting file, when
grading. It means that your tool needs to be generic working with different settings with
different datafile.
Functionality
Your tool must implement minimum of the following commands:
● open command
This command takes a parameter that is the name of the json file. It opens the json file and
reads the given settings. Then, it reads the whole index file in to memory and waits for the
further commands.
● create_index command
This command creates the index file using key field which is specified in the json file. You
need to implement the simple indexing method and you should use the relative record number
or absolute byte address as pointer. You can use any sorting algorithm you wish.
● search command
This command finds the record for the given key in index file using binary search and fetches
corresponding record from datafile.
● close command
This command closes the used files and frees the memory.
