#include <iostream>
using namespace std;

//����������ý��뺯��������޸ģ���������������Ƿ��޸ģ�
void ChangeRef(int& ref){
    ref++;
    cout << "�������޸ĺ��ֵΪ:" << ref << endl;
}

//�����Դ�ֵ��ʽ���뺯�������޸ģ���������������Ƿ��޸�
void ChangeNotRef(int ref){
    ref++;
    cout << "ͨ����ֵ��ʽ�ں������޸ĺ��ֵΪ:" << ref << endl;
}

//������ָ��Ϊ�������뺯�����޸�ָ��ָ���Ƿ�Ӱ���ⲿָ��ָ��
void ChangePtr(int* ptr){
    ptr++;
    cout << "�������޸ĺ��ָ��ָ��" << ptr << endl;
}

//�����ں������޸�ָ��ָ������ݣ��Ƿ�Ӱ���ⲿָ��ָ������
void ChangePtrVal(int* ptr){
    (*ptr)++;
    cout << "�������޸ĺ�ָ��ָ���ֵ��" << *ptr << endl;
}

#if 0
int main(){
    int Ref = 0;
    cout << "��������֮ǰ��ֵΪ:" << Ref << endl;
    ChangeRef(Ref);
    cout << "�ں������޸����ú�����������ֵΪ:" << Ref << endl<<endl;

    ChangeNotRef(Ref);
    cout << "ͨ����ֵ��ʽ�ں������޸ĺ�����������ֵΪ:" << Ref << endl<<endl;

    int* ptr = &Ref;
    cout << "���뺯��ǰptr�ĵ�ַ��" << &ptr << endl;

    cout << "���뺯��ǰ��ָ��ָ��" << ptr << endl;
    ChangePtr(ptr);
    cout << "��ֵ��ʽ���뺯���������ָ��ָ��" << ptr << endl<<endl;

    cout << "����ǰָ��ָ���ֵΪ��" << *ptr << endl;
    ChangePtrVal(ptr);
    cout << "�Ӻ����г�����ָ��ָ���ֵΪ��" << *ptr << endl<<endl;

    char *cTestTemp = "hello coco!";
    int *ppppTemp = (int *)&cTestTemp;

    cout << "ppppTemp��ֵΪ��" << ppppTemp << endl;
    cout << "ppppTemp�ĵ�ַΪ��" << &ppppTemp << endl;
    cout << "ppppTempָ�������Ϊ��" << *ppppTemp << endl;

    cout << "cTestTemp��ֵΪ��" << cTestTemp << endl;
    cout << "cTestTemp�ĵ�ַΪ��" << &cTestTemp << endl;
    cout << "cTestTempָ�������Ϊ��" << *cTestTemp << endl;

    int ref = 32222;
    int* ptrTemp = &ref;
    cout << "���뺯��ǰref���ݣ�" << ref << endl;
    cout << "���뺯��ǰref��ָ��ָ��" << &ref << endl;
    cout << "���뺯��ǰref��ָ��ָ����ߵ����ݣ�" << *(&ref) << endl;

    cout << "���뺯��ǰptrTemp�ĵ�ַ��" << ptrTemp << endl;
    cout << "���뺯��ǰptrTemp��ָ��ָ��" << &ptrTemp << endl;
    cout << "����ǰָ��ptrTempָ���ֵΪ��" << *ptrTemp << endl;


    system("pause");
}
#endif
