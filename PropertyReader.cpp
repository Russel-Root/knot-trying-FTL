#include "PropertyReader.h"
#include <string>

PropertyReader::PropertyReader(){
	this->properties = new MyLinkedList<Entry>();
	this->iterator = new MyLinkedListIterator<Entry>(this->properties);
}

PropertyReader::~PropertyReader(){
	delete this->properties;
	delete this->iterator;
}

bool PropertyReader::readConfigFile(const char* filePath){
	//////////////////////////////////////////////////////////////////////////
	if(!filePath)
		return false;
	FILE * file = fopen(filePath, "r");
	if(!file)
		return false;
	bool flag = true;// error flag
	char * key = new char[KEY_MAX_LENGTH + 1];// hold the key part
	char * value = new char[VALUE_MAX_LENGTH + 1];// hold the value part
	int length, i;// used for the array <temp>
	char * temp = new char[1024];// buffer for reading from file
	char * part = key;// read in the key part first
	int index = -1;// the index of the last character in <key> or <value>
	Entry * entry;
	while((length = fread(temp, sizeof(char), 1024, file)) > 0){
		for(i = 0; i < length; i ++){
			if(temp[i] == ' ' || temp[i] == '\t')
				continue;// skip blank space and tab

			if(part == key){// read in the key part
				do{
					if(temp[i] == '\n' || temp[i] == '\r'){
						// end of line is reached,
						// not key-value pair format
						i ++;
						index = -1;
						continue;
					}
					else if(temp[i] == '#'){
						i ++;// skip '#'
						while(i < length){
							if(temp[i] != '\n' && temp[i] != '\r')
								i ++;
							else
								break;
						}
						i ++;// skip '\n' or '\r'
						index = -1;
						continue;
					}
					else if(temp[i] == '='){
						// the end of the key part is reached
						if(index < 0){// empty key part
							flag = false;
							break;
						}
						// delete the blank space and tabs at the end of the key
						while(index > 0)
							if(key[index] == ' ' || key[index] == '\t')
								index --;
							else
								break;
						key[index + 1] = '\0';

						// forword to the value part
						part = value;
						index = -1;
						break;
					}
					part[++index] = temp[i ++];// copy
					if(index + 1 >= KEY_MAX_LENGTH){// the name of the key is too long
						flag = false;
						break;
					}
				}while(i < length);
				if(!flag)// bad format
					break;
			}
			else{// read in the value part
				do{
					if(temp[i] == '#'){
						i ++;// skip '#'
						while(i < length){
							if(temp[i] != '\n' && temp[i] != '\r')
								i ++;
							else
								break;
						}
						//i ++;// skip '\n' or '\r'
						continue;
					}
					else if(temp[i] == '\n' || temp[i] == '\r'){
						// end of line is reached
						// delete the blank space and tabs at the end of the value
						while(index > 0)
							if(value[index] == ' ' || value[index] == '\t')
								index --;
							else
								break;
						value[index + 1] = '\0';
						// create and add one new entry
						entry = Entry::createNewEntry(key, value);
						this->properties->add(entry);

						// forward to the next key part
						part = key;
						index = -1;
						break;
					}
					part[++index] = temp[i ++];// copy
					if(index + 1 >= VALUE_MAX_LENGTH){
						flag = false;
						break;
					}
				}while(i < length);

				if(flag && feof(file) && part == value){//check for end of file
					// end of line is reached
					// delete the blank space and tabs at the end of the value
					while(index > 0)
						if(value[index] == ' ' || value[index] == '\t')
							index --;
						else
							break;
					value[index + 1] = '\0';
					// create and add one new entry
					entry = Entry::createNewEntry(key, value);
					this->properties->add(entry);
				}
			}
		}
		// check for end of file and error
		if(!flag)
			break;
		else if(feof(file))
			break;
		else if(ferror(file)){
			flag = false;
			break;
		}
	}

	delete []key;
	delete []value;
	delete []temp;
	fclose(file);// close the file
	return flag;
	//////////////////////////////////////////////////////////////////////////
	//if ( !filename )
	//	return false;

	//FILE* file = fopen(filename, "r");
	//if ( !file )
	//	return false;

	//bool flag = true; // error flag
	//char* key = new char[KEY_MAX_LENGTH + 1];
	//char* value = new char[VALUE_MAX_LENGTH + 1];

	//int length, i; // for the array<temp>
	//char* temp = new char[1024]; // buffer for reading from file
	//char* part = key; // read in the key first
	//int index = -1; // the index of the last character in <key> or <value>
	//Entry* entry;

	////int fSize = fseek(file, 0, SEEK_END);
	//while ( (length = fread(temp, sizeof(char), 1024, file)) > 0 ){
	//	for ( i = 0; i < length; i++ ){
	//		if ( temp[i] == ' ' || temp[i] == '\t' )
	//			continue;

	//		// part == key
	//		if ( part == key ){ // part��Ϊ��ʶ�������ȶ�part��ֵΪkey����ʾ��ʼ��ȡkey���֣�key���ֶ�ȡ�����󣬸�part��ֵvalue
	//			do{
	//				if ( temp[i] == '\n' || temp[i] == '\r' ){
	//					i++;
	//					index = -1;
	//					continue;
	//				}
	//				// ��configuration files��#��ע�ͷ��š����һ���ҵ���#����ô����ȥ�������ַ���ֱ������������\n����\r
	//				else if ( temp[i] == '#' ){ 
	//					i++;
	//					while ( i < length ){ // ����while�ĳ�����temp[i] == '\n' && temp[i] == '\r'
	//						if ( temp[i] != '\n' && temp[i] != '\r')
	//							i++;
	//						else
	//							break;
	//					}
	//					i++;
	//					index = -1; // ��index��ֵΪ-1��continue�˳������do whileѭ��������Ѱ��key��ֵ
	//					continue;
	//				}
	//				// �ҵ���������keyֵ
	//				else if ( temp[i] == '=' ){ 
	//					if ( index < 0 ){
	//						flag = false;
	//						break;
	//					}
	//					while ( index > 0 )
	//						if (key[index] == ' ' || key[index] == '\t') // delete the blank space and tabs at the end of the key
	//							index--;
	//						else 
	//							break;
	//					key[index + 1] = '\0'; // \0��ʾkey������
	//					////////////////////////////////////////////////////////////////////////// exit
	//					part = value; // �ҵ���������keyֵ�󣬿�ʼѰ��value��ֵ��������if(part == key)�ĳ���
	//					index = -1;
	//					break;
	//				}
	//				//////////////////////////////////////////////////////////////////////////
	//				part[++index] = temp[i++]; // ���ҵ���keyֵ����part����part = key�����char* key��ʼ��ֵ
	//				if ( index + 1 > KEY_MAX_LENGTH ){
	//					flag = false;
	//					break;
	//				}
	//			}while(i < length);
	//			if ( !flag )
	//				break;
	//		}
	//		////////////////////////////////////////////////////////////////////////// part == value
	//		else{
	//			do{
	//				if (temp[i] == '#'){
	//					i++;
	//					while (i < length){
	//						if (temp[i] != '\n' && temp[i] != '\r')
	//							i++;
	//						else
	//							break;
	//					}
	//					continue;
	//				}
	//				// �ҵ���������valueֵ
	//				else if (temp[i] == '\n' || temp[i] == '\r'){
	//					while (index > 0)
	//						if ( value[index] == ' ' || value[index] == '\t' )
	//							index--;
	//						else
	//							break;
	//					value[index + 1] = '\0';

	//					// create and add one new entry
	//					entry = Entry::createNewEntry(key, value);
	//					this->properties->add(entry);

	//					// forward to the next key part ////////////////////////// exit
	//					part = key; // ������ if (part == key) else ��else�ĳ���
	//					index = -1;
	//					break;
	//				}
	//				//////////////////////////////////////////////////////////////////////////
	//				part[++index] = temp[i++];
	//				if ( index + 1 >= VALUE_MAX_LENGTH ){
	//					flag = false;
	//					break;
	//				}
	//			}while(i < length);

	//			// check for the end of the file
	//			if ( flag && feof(file) && part == value ){
	//				while (index > 0)
	//					if ( value[index] == ' ' || value[index] == '\t')
	//						index--;
	//					else
	//						break;
	//				value[index + 1] = '\0';

	//				entry = Entry::createNewEntry(key, value);
	//				this->properties->add(entry);
	//			}
	//		}
	//	}
	//	if ( !flag )
	//		break;
	//	else if ( feof(file) )
	//		break;
	//	else if ( ferror(file) ){
	//		flag = false;
	//		break;
	//	}
	//}
	//delete[] key;
	//delete[] value;
	//delete[] temp;
	//fclose(file);
	//return flag;
}
             
const char* PropertyReader::getProperty(const char* key) const{
	char* value = NULL;
	Entry* entry;
	this->iterator->rewind();

	while ( this->iterator->hasNext() ){
		entry = this->iterator->next();
		if ( strcmp(entry->key, key) == 0 ){
			value = entry->value;
			break;
		}
	}
	return value;
}