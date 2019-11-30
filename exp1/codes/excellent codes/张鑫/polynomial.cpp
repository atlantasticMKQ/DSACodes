#include "polynomial.h"
#include <cmath>
#include <iostream>
polynomial::polynomial()
{
	header = new Node;
	header->next = nullptr;
	length = 0;
}

polynomial::polynomial(const polynomial& p) {
	this->copy(p);
}
void polynomial::operator=(const polynomial& p) {
	this->copy(p);
}
polynomial::~polynomial()
{
	if (header!=nullptr)
		destroy();
}
void polynomial::clear() {		//ֻ����ͷ�ڵ�
	if (header == nullptr) return;
	Node *tmp,*tmp1= header->next;
	while (tmp1 != nullptr) {
		tmp = tmp1->next;
		delete tmp1;
		tmp1 = tmp;
	}
	header->next = nullptr;
	length = 0;
}
void polynomial::destroy() {
	clear();
	delete header;
	header = nullptr;
}
void polynomial::copy(const polynomial &dst) {			
	if (this->header == dst.header || dst.header==nullptr) return;	//��������
	Node *end,*newnode,*tmp= dst.header->next;
	if (header != nullptr) this->clear();						//�ͷ�ԭ���ڴ�
	header->next = nullptr;
	end = header;
	length = 0;
	while (tmp!= nullptr) {
		newnode = new Node(tmp->A, tmp->K);
		end->next = newnode;
		end = newnode;
		tmp = tmp->next;
		length++;
	}
	end->next = nullptr;
}
void polynomial::append(double a, int k) {			//Ҫ��ά���Ӵ�С��ָ��˳��,ɾ��С�����ֵ�Ľڵ㡣Ϊ�˲����ķ��㣬���಻ά��Ϊ��ĳ�����㣬����Ҫ���⴦���ʱ������⿼��
	if (abs(a) < 1e-8) return;						//���뼫С��ֱ�ӷ���
	Node *tmp1 = header->next,*tmp2=header, *newnode;
	newnode = new Node(a, k);
	while (tmp1 != nullptr && tmp1->K > newnode->K) {
		tmp1 = tmp1->next;
		tmp2 = tmp2->next;
	}
	if (tmp1 == nullptr) {		//�����еĶ�С
		tmp2->next = newnode;
		length++;
	}
	else if (tmp1->K == newnode->K) {			//����ͬ�ڵ�
		tmp1->A += newnode->A;
		if (abs(tmp1->A) <= 1e-8) {			//ɾ���ڵ�
			tmp2->next = tmp1->next;
			delete tmp1;
			length--;
		}
	}
	else {						//����ͬ�ڵ�
		tmp2->next = newnode;
		newnode->next = tmp1;
		length++;
	}
}

void polynomial::print() {
	if (header == nullptr) {
		printf("NULL\n"); return;
	}
	if (length == 0||header->next==nullptr) {					//ֻ��һ��������0
		printf( "0.0000\n");
		return;
	}
	Node* tmp = header->next;
	bool first = true;
	while (tmp != nullptr) {
		if (tmp->A > 0)					//����ϵ����ϵ��������
			if (first){
				printf("%.4f", tmp->A);
				first = false;
			}
			else {
				printf( "+%.4f", tmp->A);
			}
		else{							//ϵ��С����
			printf("%.4f", tmp->A);
			first = false;
		}
		if (tmp->K == 1) {				//����ָ����ָ������1
			printf( "x");
		}
		else if(tmp->K!=0){
			printf("x^%d",tmp->K);
		}
		tmp = tmp->next;
	}
	printf("\n");
}
void polynomial::add(polynomial& m1, polynomial& m2) {
	if (m1.header == nullptr || m2.header == nullptr) {				//��������������һ�������ڵĴ���
		return;
	}
	if (m1.header == this->header) {		//���ĳ��������thisһ������ת��Ϊ��Ŀ������ʽ��add
		this->add(m2);
		return;
	}
	if (m2.header == this->header) {		//ͬ��
		this->add(m1);
		return;
	}
	this->clear();							//����ڴ�
	this->add(m1);
	this->add(m2);
}
void polynomial::add(polynomial& m1) {
	if (m1.header == this->header) {		//�Լ����Լ������ڳ˶�
		Node* tmp = header->next;
		while (tmp != nullptr) {
			tmp->A += tmp->A;
			tmp = tmp->next;
		}
		return;
	}
	polynomial tmp = polynomial();
	tmp.copy(m1);
	Node* tmp1 = this->header;
	Node* tmp2 = tmp.header->next;
	while (tmp2 != nullptr) {				//��m1�������Լ����ϲ�
		while (tmp1->next!=nullptr && tmp1->next->K > tmp2->K)
			tmp1 = tmp1->next;
		if (tmp1->next == nullptr) {		//ʣ���ֱ�Ӳ��β
			tmp1->next = tmp2;
			tmp.header->next = nullptr;
			length += tmp.length;
			return;
		}
		if (tmp1->next->K == tmp2->K) {			//��ͬ�����Ľڵ�
			tmp1->next->A += tmp2->A;
			if (abs(tmp1->next->A) < 1e-8) {		//����Ӻ�Ľڵ�Ϊ�㣬��ɾ֮
				Node* tmp = tmp1->next;
				tmp1->next = tmp->next;
				delete tmp;
				length--;
			}
			tmp.header->next = tmp2->next;
			delete tmp2;
		}
		else {									//����ͬ�����Ľڵ�
			tmp.header->next = tmp2->next;
			tmp2->next = tmp1->next;
			tmp1->next = tmp2;
			length++;
		}
		tmp2 = tmp.header->next;
	}
}
void polynomial::sub(polynomial& m1, polynomial& m2){
	if (m1.header == nullptr || m2.header == nullptr) {				//��������������һ�������ڵĴ���
		return;
	}
	if (m1.header == this->header) {		//���ĳ��������thisһ������ת��Ϊ��Ŀ������ʽ��sub
		this->sub(m2);
		return;
	}
	if (m2.header == this->header) {		//ͬ��
		this->sub(m1);
		Node *tmp = header->next;
		while (tmp != nullptr) {
			tmp->A = -tmp->A;
			tmp = tmp->next;
		}
		return;
	}
	this->clear();							//����ڴ�
	this->add(m1);
	this->sub(m2);
}
void polynomial::sub(polynomial& m1) {
	if (m1.header == this->header) {		//�Լ����Լ�
		m1.clear();
		return;
	}
	polynomial tmp = polynomial();
	tmp.copy(m1);
	Node* nod =tmp.header->next;				//����-1תΪ�ӷ�
	while (nod != nullptr) {
		nod->A = -nod->A;
		nod = nod->next;
	}
	this->add(tmp);
}
void polynomial::mult(polynomial& m1, polynomial& m2) {
	if (m1.header == nullptr || m2.header == nullptr) {				//��������������һ�������ڵĴ���
		std::cout << "error:the polynomial hasn't been initialized" << std::endl;
		return;
	}
	if (m1.header == this->header) {		//���ĳ��������thisһ������ת��Ϊ��Ŀ������ʽ��mult
		this->mult(m2);
		return;
	}
	if (m2.header == this->header) {		//ͬ��
		this->mult(m1);
		return;
	}
	this->clear();							//����ڴ�
	this->copy(m1);
	this->mult(m2);
}
void polynomial::mult(polynomial& m1) {
	if (m1.header == this->header) {		//�Լ����Լ�
		polynomial tmp = polynomial();
		tmp.copy(m1);
		this->mult(tmp);
		return;
	}
	polynomial tmp_poly = polynomial();
	Node* tmpm1 = m1.header->next;
	while (tmpm1 != nullptr) {
		Node* tmpm2 = this->header->next;
		while (tmpm2 != nullptr) {
			tmp_poly.append(tmpm1->A*tmpm2->A, tmpm1->K + tmpm2->K);
			tmpm2 = tmpm2->next;
		}
		tmpm1 = tmpm1->next;
	}
	this->copy(tmp_poly);
}

double polynomial::calculate(double x) {
	double result = 0;
	Node* tmp = header->next;
	while (tmp != nullptr) {
		result += tmp->A*pow(x, tmp->K);
		tmp = tmp->next;
	}
	return result;
}

void polynomial::differential(polynomial& m) {
	polynomial tmp = polynomial();
	tmp.copy(m);
	this->clear();
	Node* nod = tmp.header->next;
	while (nod != nullptr) {
		this->append(nod->A*nod->K, nod->K - 1);
		nod = nod->next;
	}
}

void polynomial::integral(polynomial& m) {
	polynomial tmp = polynomial();
	tmp.copy(m);
	this->clear();
	Node* nod = tmp.header->next;
	while (nod != nullptr) {
		this->append(nod->A / (double)(nod->K+1), nod->K + 1);
		nod = nod->next;
	}
}

double polynomial::definite_integral(double x1, double x2) {
	polynomial tmp = polynomial();
	tmp.copy(*this);
	tmp.integral(tmp);
	return tmp.calculate(x2) - tmp.calculate(x1);
}

void polynomial::divide(polynomial& m1, polynomial& m2) {
	if (m1.header == nullptr || m2.header == nullptr) {				//������������һ�������ڵĴ���
		std::cout << "error:the polynomial hasn't been initialized" << std::endl;
		return;
	}
	if (m1.header == this->header) {		//���ĳ������thisһ������ת��Ϊ��Ŀ������ʽ��divide
		this->divide(m2);
		return;
	}
	if (m2.header == this->header) {		//ͬ��
		this->divide(m1);
		return;
	}
	this->clear();							//����ڴ�
	this->copy(m1);
	this->divide(m2);
}

void polynomial::divide(polynomial& m) {	
	if (m.header->next == nullptr) {
		std::cout << "error:divide zero!" << std::endl;  //����Ϊ��
		return;
	}
	if (m.header == this->header) {		//�Լ����Լ�
		this->clear();
		this->append(1, 0);
		return;
	}
	polynomial quotient = polynomial();		//quotient��������
	polynomial tmp_poly = polynomial();		//�����м�������
	Node* tmp = header->next;
	double a1 = tmp->A;
	double a2 = m.header->next->A;
	while (tmp != nullptr && tmp->K >= m.header->next->K) {		//���������ϵ����С�ڳ������Լ�����
		double A = a1 / a2;
		int K = tmp->K - m.header->next->K;						//�õ�ϵ��ΪA��ָ��ΪK����
		quotient.append(A,K);
		Node* tp = m.header->next;									
		while (tp != nullptr) {									//���õ������������ˣ��õ��м���
			tmp_poly.append(A*tp->A,K+tp->K);
			tp = tp->next;
		}
		this->sub(tmp_poly);									//��ȥ�м���
		tmp = header->next;										//����
		if (tmp!=nullptr) a1 = tmp->A;							//����
		tmp_poly.clear();										//����
	}
	this->copy(quotient);
}


void polynomial::mod(polynomial& m1, polynomial& m2) {
	if (m1.header == nullptr || m2.header == nullptr) {				//������������һ�������ڵĴ���
		std::cout << "error:the polynomial hasn't been initialized" << std::endl;
		return;
	}
	if (m1.header == this->header) {		//���ĳ������thisһ������ת��Ϊ��Ŀ������ʽ��mod
		this->mod(m2);
		return;
	}
	if (m2.header == this->header) {		//ͬ��
		this->mod(m1);
		return;
	}
	this->clear();							//����ڴ�
	this->copy(m1);
	this->mod(m2);
}


void polynomial::mod(polynomial& m) {
	if (m.header->next == nullptr) {
		std::cout << "divide zero!" << std::endl;  //����Ϊ��
		return;
	}
	if (m.header == this->header) {		//�Լ�ģ�Լ�
		this->clear();
		return;
	}
	polynomial tmp_poly = polynomial();		//�����м�������
	Node* tmp = header->next;
	double a1 = tmp->A;
	double a2 = m.header->next->A;
	while (tmp != nullptr && tmp->K >= m.header->next->K) {		//���������ϵ����С�ڳ������Լ�����
		double A = a1 / a2;
		int K = tmp->K - m.header->next->K;						//�õ�ϵ��ΪA��ָ��ΪK����
		Node* tp = m.header->next;
		while (tp != nullptr) {									//���õ������������ˣ��õ��м���
			tmp_poly.append(A*tp->A, K + tp->K);
			tp = tp->next;
		}
		this->sub(tmp_poly);									//��ȥ�м���
		tmp = header->next;										//����
		if (tmp != nullptr) a1 = tmp->A;						//����
		tmp_poly.clear();										//����
	}
	//���µ�this��������
}

void polynomial::GCD(polynomial& m1, polynomial& m2) {			//����շת����������Լʽ
	if (m1.header == nullptr || m2.header == nullptr) {				//������������һ�������ڵĴ���
		std::cout << "error:the polynomial hasn't been initialized" << std::endl;
		return;
	}
	polynomial p1 = polynomial();
	polynomial p2 = polynomial();
	polynomial p = polynomial();
	p1.copy(m1);
	p2.copy(m2);
	p.mod(p1, p2);
	while (p.length != 0) {						//ֱ������ʽΪ������
		p1.copy(p2);
		p2.copy(p);
		p.mod(p1, p2);
	}
	double A;									//��������p2���й�һ������
	Node* tmp = p2.header->next;
	if (tmp != nullptr)
		A = tmp->A;
	while (tmp != nullptr) {
		tmp->A /= A;
		tmp = tmp->next;
	}
	this->copy(p2);
}

void polynomial::LCM(polynomial& m1, polynomial& m2) {
	if (m1.header == nullptr || m2.header == nullptr) {				//������������һ�������ڵĴ���
		std::cout << "error:the polynomial hasn't been initialized" << std::endl;
		return;
	}
	polynomial p1 = polynomial();
	polynomial p2 = polynomial();
	polynomial p = polynomial();
	p1.copy(m1);
	p2.copy(m2);
	p.GCD(m1, m2);
	this->mult(p1, p2);
	this->divide(p);							//��С����ʽ�����Լʽ����p1*p2
	double A;									//��������this���й�һ������
	Node* tmp = this->header->next;
	if (tmp != nullptr)
		A = tmp->A;
	while (tmp != nullptr) {
		tmp->A /= A;
		tmp = tmp->next;
	}
}

void polynomial::exponentiation(polynomial& m, int x) {
	polynomial tmp = polynomial();
	tmp.copy(m);
	this->clear();
	while (x > 0) {
		if (x % 2 == 1) {
			if (this->header->next == nullptr)
				this->copy(tmp);
			else this->mult(tmp);
		}
		tmp.mult(tmp);
		x = x / 2;
	}
}