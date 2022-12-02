#include "Controller.h"
#include <iostream>
using namespace std;

// --- Singleton constructor --- 
Controller* Controller::controller = nullptr;
Controller::Controller(string employeeFile, string departmentFile, Helper& h)
	: empDAO(new EmployeeDAO(employeeFile)),
	  dptDAO(new DepartmentDAO(departmentFile)),
	  helper(h) {};

Controller *Controller::getInstance(string employeeFile, string departmentFile, Helper& helper) {
	if(controller == nullptr)
		controller = new Controller(employeeFile, departmentFile, helper);
	return controller;
}

// --- Bussiness public member functions ---
void Controller::displayHelp() {
	helper.help();
}

void Controller::displayHelp(string command) {
	helper.help(command);
}

// Yet to be added most features
void Controller::lsEmployees() {
	using std::cout;
	vector<Employee*> list = empDAO->findAll();
	for (Employee* e : list)
		cout << e->toString() << endl;
}

void Controller::lsDepartments() {
	vector<Department*> list = dptDAO->findAll();
	for (Department* d : list)
		cout << d->toString() << endl;
}

// --- DB-Related public member functions ---
optional<Employee*> Controller::findEmployee(int id) {
	return empDAO->find(id);
}
vector<Employee*> Controller::findEmployees() {
	return empDAO->findAll();
}        
bool Controller::createEmployee(std::string name, int departmentId) {
	int id = selectEmployeeId();
	if(!checkDptId(departmentId)) return false;
	Employee employee(id, name, departmentId);
	empDAO->create(employee);
	return true;
}
void Controller::removeEmployee(int id) {
	empDAO->deletion(id);
}
bool Controller::updateEmployee(int id, string name, int departmentId) {
	if(!checkDptId(departmentId)) return false;
	Employee employee(id, name, departmentId);
	empDAO->update(employee);
	return true;
}
optional<Department*> Controller::findDepartment(int id) {
	return dptDAO->find(id);
}
vector<Department*> Controller::findDepartments() {
	return dptDAO->findAll();
}        	
bool Controller::createDepartment(std::string name, int sells, int managerId) {
	int id = selectDepartmentId();
	if(!checkManagerId(managerId)) return false;
	Department department(id, name, sells, managerId);
	dptDAO->create(department);
	return true;
}
void Controller::removeDepartment(int id) {
	dptDAO->deletion(id);
}
bool Controller::updateDepartment(int id, string name, int sells, int managerId) {
	if(!checkManagerId(managerId)) return false;
	Department department(id, name, sells, managerId);
	dptDAO->update(department);
	return true;
}

// --- Private member functions ---
// No sé si hacer uno para empleados y departamentos o uno para cada uno
int Controller::selectEmployeeId() {
	return empDAO->maxId();
}

int Controller::selectDepartmentId() {
	return dptDAO->maxId();
}

bool Controller::checkManagerId(int id) {
	return empDAO->idExists(id);
}

bool Controller::checkDptId(int id) {
	return dptDAO->idExists(id);
}