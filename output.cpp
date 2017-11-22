#include "assembler.hpp"

/* prototype declaring of the functions */
void showSensorData(Sensor);
void showEvolvingTimestamp(Sensor);
void showEvolvingInterval(Sensor);
void showEvolvingSegment(Sensor);
void showSegmentCluster(Sensor);
void exportCsvFile(Sensor (&)[M]);

/* main */
void outputData(Sensor (&s)[M]){
  int mode;
	int number;
	while(mode != -1){
		cout << endl << "please select the output mode"
         << endl << "1:data, 2:evolving, 3:interval, 4:segment, 5:cluster, 6:export, -1:exit "
         << endl << "mode = " ;
		cin >> mode;
    if(mode == -1) break;
    if(mode < 6){
      cout << "please type the sensor number ( 0 ~ 179 ) : ";
      cin >> number;
    }
    switch(mode){
      case 1: showSensorData(s[number]); break;
      case 2: showEvolvingTimestamp(s[number]); break;
      case 3: showEvolvingInterval(s[number]); break;
      case 4: showEvolvingSegment(s[number]); break;
      case 5: showSegmentCluster(s[number]); break;
      case 6: exportCsvFile(s); break;
      default : cout << "mode number is wrong" << endl; break;
    }
	}
}

void showSensorData(Sensor s){
  for(int n=0;n<N;n++){
    cout << n << " " << s.getPM25()[n] << endl;
  }
  cout << "----------------------------------------" << endl
       << "ID = " << s.getID() << endl
       << "----------------------------------------" << endl;
}

void showEvolvingTimestamp(Sensor s){
  vector<int> ets = s.getETS();
  auto itr = ets.begin();
  while(itr != ets.end()){
    cout << *itr << endl;
    itr++;
  }
  cout << "----------------------------------------" << endl
       << "ID = " << s.getID() << endl
       << ets.size() << " EvolvingTimestamps" << endl
       << "----------------------------------------" << endl;
}

void showEvolvingInterval(Sensor s){
    map<int,int> interval = s.getINTERVAL();
    auto itr = interval.begin();
    while(itr != interval.end()){
      cout << itr->first << " --> "<< itr->second << endl;
      itr = interval.lower_bound(itr->second);
    }
    cout << "----------------------------------------" << endl
         << "ID = " << s.getID() << endl
         << s.getETS().size() << " EvolvingTimestamps" << endl
         << interval.size() << " Interval" << endl
         << "----------------------------------------" << endl;
}

void showEvolvingSegment(Sensor s){
  map<int,int> interval = s.getINTERVAL();
  map<int,int> segment = s.getSEGMENT();
  auto interval_itr = interval.begin();
  while(interval_itr != interval.end()){
    cout << interval_itr->first << " --> " << interval_itr->second << endl;
    int i = interval_itr->first;
    while(i < interval_itr->second){
      cout << " | " << i << " --> " << segment[i] << " slope = " << s.getSLOPE_VALUE(i)<< endl;
      i = segment[i];
    }
    cout << endl;
    interval_itr++;
  }
  cout << "----------------------------------------" << endl
       << "ID = " << s.getID() << endl
       << segment.size() << " segments" << endl
       << "----------------------------------------" << endl;
}

void showSegmentCluster(Sensor s){
  multimap<double,int> cluster = s.getCLUSTER();
  auto cluster_itr = cluster.begin();
  double prev = 0.0;
  int count = 0;
  while(cluster_itr != cluster.end()){
    if(cluster_itr->first != prev){
      count++;
      cout << "cluster " << count
           << " lower= " << s.getLOWER(cluster_itr->first)
           << " upper= " << s.getUPPER(cluster_itr->first)
           << endl;
      prev = cluster_itr->first;
    }
    cout << "| " << cluster_itr->second
         << " slope= " << s.getSLOPE_VALUE(cluster_itr->second)<< endl;
    cluster_itr++;
  }
  cout << "----------------------------------------" << endl
       << "ID = " << s.getID() << endl
       <<  s.getSEGMENT().size() << " segments" <<endl
       << count << " clusters" << endl
       << "----------------------------------------" << endl;
}

void exportCsvFile(Sensor (&s)[M]){
  int start, end;
  string path = "C:\\Users\\OnizukaLab\\Documents\\workspase\\SantanderSTDM\\output\\";
  cout << "please specify the range of sensor number ( 0 ~ 179 )" << endl;
  cout << "start = ";
  cin >> start;
  cout << "end = ";
  cin >> end;
  if(0<=start && start<=179 && 0<=end && end<=179){
    cout << "exorting ... ";
    ofstream ofs(path+"data_"+to_string(start)+"-"+to_string(end)+".csv");
    if(!ofs){
      cout << "failed to open the file" << endl;
      exit(1);
    }
    for(int n=0;n<N;n++){
      ofs << n;
      for(int m=start;m<=end;m++){
        ofs << "," << s[m].getPM25()[n];
      }
      ofs << endl;
    }
    cout << "finished !!" << endl;
  }else{
    cout << "sensor number is wrong" << endl;
  }
}
