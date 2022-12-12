#include "ThreadSafeSet.cpp"
#include "ThreadSafeSet.h"
#include <chrono>
#include <thread>

void reader(int _id, int _max_number);
void writer(int _i,int _max_number);

ThreadSafeSet<int> test;

int main(/* int argc, char *argv[] */){
	std::chrono::time_point<std::chrono::system_clock> start, end;

	int thread_num=5;
	int max_number=1000;
	thread all_reader[thread_num];
	thread all_writer[thread_num];
	
	cout << "Adding " << max_number << endl;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < thread_num; i++){
		all_writer[i] = thread(writer,i,max_number);
		all_reader[i] = thread(reader, i, max_number);
	}

	for (int i = 0; i < thread_num; i++){
		all_writer[i].join();
		all_reader[i].join();
	}

	end = std::chrono::system_clock::now();
	cout<<"Added"<<endl;
	std::chrono::duration<double> elapsed_seconds = end - start;
	cout<<"Time passed: "<< elapsed_seconds.count()<<" seconds"<<endl; 
	cout<<"Size: "<< test.count()<<endl;
	test.print();
	cout << "Size: " << test.count() << endl;

	/* cout << "Before clear(): " << test.count() <<endl;
	test.clear();
	cout << "After clear(): " << test.count() << endl; */

	//test.iterate([](int val ) {cout<<test.find(val);});
	return 0;
}
void reader(int _id, int _max_number){
	int id = _id, max_number = _max_number;
	for (int i = 0; i <max_number; i++){
		cout << "reader " << id << " searches " << i << " : " << test.find(i) << endl;
	}
}
void writer(int _id,int _max_number){
	int id=_id,max_number=_max_number;
	if (id%2==0){
		for (int i = 0; i < max_number; i++)
			cout << "writer " << id << " adds " << i << " : " << test.insert(i) << endl;
	}else
		for (int i = 0; i < max_number; i++)
			cout << "writer " << id << " removes " << i << " : " << test.remove(i) << endl;
}