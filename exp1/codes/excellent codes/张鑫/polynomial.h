#pragma once
#include<fstream>
struct Node {
	double A;
	int K;
	struct Node* next;
	Node(double a, int k) {
		this->A = a;
		this->K = k;
		this->next = nullptr;
	}
	Node() {
		this->A = 0;
		this->K = 0;
		this->next = nullptr;
	}
};

class polynomial
{
public:
	Node* header;
	int length;
public:
	polynomial();
	~polynomial();
	void operator=(const polynomial&);
	polynomial(const polynomial&);
	void copy(const polynomial&);						//����
	void append(double, int);					//����ϵ�����������ڵ�������ʽ����
	void print();								//��ӡ���
	void add(polynomial&, polynomial&);			//��Ŀ�ӷ�
	void add(polynomial&);						//һĿ�ӷ�
	void sub(polynomial&, polynomial&);			//��Ŀ����
	void sub(polynomial&);						//һĿ����
	void mult(polynomial&, polynomial&);		//��Ŀ�˷�
	void mult(polynomial&);						//һĿ�˷�
	double calculate(double);					//��X������ֵ
	void destroy();								//��������������
	void clear();								//���
	void differential(polynomial&);				//΢��
	void integral(polynomial&);					//��������
	double definite_integral(double,double);	//������				
	void divide(polynomial&, polynomial&);		//��Ŀ����
	void divide(polynomial&);					//һĿ����
	void mod(polynomial&, polynomial&);			//��Ŀȡģ
	void mod(polynomial&);						//һĿȡģ
	void GCD(polynomial&, polynomial&);			//���Լʽ
	void LCM(polynomial&, polynomial&);			//��С����ʽ
	void exponentiation(polynomial&, int);		//����ʽ���ݣ����ÿ������㷨
};

