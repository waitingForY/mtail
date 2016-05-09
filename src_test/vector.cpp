#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
	vector<int> vet_i;
	for(int i=0;i<40;i++)
	{
		cout<<"before insert!"<<endl;
		cout<<"vet_i: size="<<vet_i.size()<<endl;
		cout<<"vet_i:capacity="<<vet_i.capacity()<<endl;
		vet_i.push_back(i);
		cout<<"after insert!"<<endl;
		cout<<"vet_i: size="<<vet_i.size()<<endl;
		cout<<"vet_i:capacity="<<vet_i.capacity()<<endl;
	}
	return 0;
}

