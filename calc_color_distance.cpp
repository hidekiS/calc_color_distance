//============================================================================
// Name        : calc_color_distance.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#define foreach BOOST_FOREACH

using namespace std;
//calc color distance between db and key
double calc_distance(list<string> db,list<string> key){
	double dbelem,keyelem,distance=0;
	double sigma=1; //weight of Cr
	double tau = 1;
	list <string>::iterator dbi = db.begin();
	list <string>::iterator keyi = key.begin();
	list <double> time;
	dbi ++; keyi++;
	while(!(*dbi == db.back())){
		dbelem = boost::lexical_cast<double>(*dbi);
		keyelem = boost::lexical_cast<double>(*keyi);
		time.push_back((dbelem-keyelem)*(dbelem-keyelem));
		dbi ++; keyi++;
	}
	list <double>::iterator timei = time.begin();
	distance += *timei++; distance += sigma * *timei++;	distance += tau * *timei++;
	distance += tau * *timei++;	distance += tau * sigma * *timei++;	distance += tau * sigma * *timei;
	//distance = sqrt(time.get_allocator(0) + sigma * time[1] + tau*(time[2]+time[3] + tau*sigma*(time[4]+time[5]));
	distance = sqrt(distance);
	return distance;
}

void fwrite_distance(list< list<string> > llstr,list<double> distance){
	char tmpstr[1024];
	sprintf(tmpstr,"color_distance.ods");

	//ofstreamクラスを使って表計算ファイルを書き込みオープン
	ofstream ofs;
	ofs.open(tmpstr,ios::out);
	if( !ofs ){
		cout << "Error: cannot open file(" << tmpstr << ")" << endl;
		return ;
	}

	ofs << " ";
	BOOST_FOREACH(list<string> lstr, llstr){
		list <string>::iterator lstri = lstr.begin();
		ofs << *lstri <<"\t";
	}
	ofs << endl;

	//類似度行列を出力
	list <double>::iterator distancei = distance.begin();
	BOOST_FOREACH(list<string> lstr, llstr){
		list <string>::iterator lstri = lstr.begin();
		ofs << *lstri <<"\t";
		for(size_t i=0;i<llstr.size();i++){
			ofs << *distancei <<"\t";
			distancei++;
		}
		ofs << endl;
		lstri++;
	}

	ofs.close();
}

int main(int argc, char **argv) {

	//ofstreamクラスを使って平均色情報ファイルを読み込みオープン
	ifstream ifs(argv[1],std::ios::in);
	if( !ifs ){
		cout << "Error: cannot open file(" << argv[1] << ")" << endl;
		return -1;
	}

	//read file
	char buf[1024];
	list<string> lstr;
	list<list<string> > llstr;
	while(ifs.getline(buf,1024)){
		string str(buf);

		//sprit string
		boost::split(lstr, str, boost::is_any_of("\t"));
		llstr.push_back(lstr);
	}
	ifs.close();

	//calc distance
	list<double> distance;
	BOOST_FOREACH(list<string> db, llstr){
		BOOST_FOREACH(list<string> key, llstr){
			distance.push_back(calc_distance(db,key));
		}
	}
	cout << "calc finish"<<endl;
	fwrite_distance(llstr,distance);

	return 0;
}
