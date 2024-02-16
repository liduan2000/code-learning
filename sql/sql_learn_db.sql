-- Active: 1699918881937@@127.0.0.1@3306@mysql_learn

CREATE DATABASE sql_learn;

USE sql_learn;

CREATE TABLE
    emp(
        id int,
        work_number varchar(10),
        name varchar(10),
        gender char(1),
        age tinyint UNSIGNED,
        id_card varchar(18),
        work_address varchar(50),
        entry_date date
    );

DESC emp;

INSERT INTO
    emp (
        id,
        work_number,
        name,
        gender,
        age,
        id_card,
        work_address,
        entry_date
    )
VALUES (
        1,
        '1',
        '柳岩',
        '女',
        20,
        123456789012345678,
        '北京',
        '2000-01-01'
    ), (
        1,
        '2',
        '张无忌',
        '男',
        18,
        123456789012345670,
        '北京',
        '2005-09-01'
    ), (
        1,
        '3',
        '赵敏',
        '女',
        25,
        123456789012345671,
        '上海',
        '2006-02-01'
    ), (
        1,
        '4',
        '杨过',
        '男',
        21,
        123456789012345672,
        '深圳',
        '2002-05-01'
    ), (
        1,
        '5',
        '小龙女',
        '女',
        15,
        123456789012345673,
        '上海',
        '2003-08-01'
    );

SELECT gender, count(*) FROM emp GROUP BY gender;

SELECT gender, avg(age) FROM emp GROUP BY gender;

SELECT work_address, count(*)
FROM emp
WHERE age < 45
GROUP BY work_address
HAVING count(*) >= 2;

SELECT * FROM emp ORDER BY age ASC, entry_date DESC;

SELECT * FROM emp LIMIT 2, 2;

SELECT *
FROM emp
WHERE
    gender = '女'
    AND age IN(20, 21, 22, 23);

SELECT *
FROM emp
WHERE
    gender = '男'
    AND age BETWEEN 20 AND 40
    AND name LIKE '___';

SELECT name, age
FROM emp
WHERE age <= 35
ORDER BY
    age ASC,
    entry_date DESC;

SELECT *
FROM emp
WHERE
    gender = '男'
    AND age BETWEEN 20 AND 40
ORDER BY
    age ASC,
    entry_date ASC
LIMIT 1;

CREATE TABLE
    user(
        id int PRIMARY KEY,
        AUTO_INCREMENT,
        name varchar(10) NOT NULL,
        UNIQUE,
    )

create table
    department(
        id int AUTO_INCREMENT PRIMARY KEY COMMENT 'ID',
        name VARCHAR(50) not null COMMENT '部门名称'
    );

insert into
    department (id, name)
values (1, '研发部'), (2, '市场部'), (3, '财务部'), (4, '销售部'), (5, '总经办');

CREATE TABLE
    employee(
        id int AUTO_INCREMENT PRIMARY KEY COMMENT 'ID',
        name VARCHAR(50) not null COMMENT '姓名',
        age TINYINT COMMENT '年龄',
        job VARCHAR(20) COMMENT '职位',
        salary int COMMENT '薪资',
        entry_date date COMMENT '入职时间',
        manager_id int COMMENT '直属领导ID',
        department_id int COMMENT '部门ID'
    );

insert into
    employee (
        id,
        name,
        age,
        job,
        salary,
        entry_date,
        manager_id,
        department_id
    )
VALUES (
        1,
        '金庸',
        66,
        '总裁',
        20000,
        '2000-01-01',
        null,
        5
    ), (
        2,
        '张无忌',
        20,
        '项目经理',
        12500,
        '2005-12-05',
        1,
        1
    ), (
        3,
        '杨晓',
        33,
        '开发',
        8400,
        '2000-11-01',
        2,
        1
    ), (
        4,
        '韦一笑',
        48,
        '开发',
        11000,
        '2003-05-01',
        2,
        1
    ), (
        5,
        '常遇春',
        43,
        '开发',
        10500,
        '2002-11-04',
        3,
        1
    ), (
        6,
        '小昭',
        19,
        '程序员鼓励师',
        6600,
        '2004-01-01',
        2,
        1
    );

select
    emp.name,
    emp.department_id,
    dept.name,
    dept.id
from
    employee emp,
    department dept
where
    emp.department_id = dept.id;

select
    emp.name,
    emp.department_id,
    dept.name,
    dept.id
from employee emp
    inner join department dept on emp.department_id = dept.id;

select
    emp.*,
    dept.name dept_name,
    dept.id dept_id
from employee emp
    left join department dept on emp.department_id = dept.id;

select
    dept.*,
    emp.name,
    emp.id emp_id
from employee emp
    right join department dept on emp.department_id = dept.id;

SELECT
    e1.id,
    e1.name,
    e2.id,
    e2.name
from employee e1
    left join employee e2 on e1.manager_id = e2.id;

SELECT *
FROM employee
WHERE entry_date > (
        SELECT entry_date
        FROM employee
        WHERE name = '常遇春'
    );

SELECT *
from employee
where department_id in (
        SELECT id
        from department
        where
            name IN('研发部', '市场部')
    );

SELECT *
FROM employee
WHERE salary > ANY (
        SELECT salary
        FROM employee
        WHERE department_id = (
                SELECT id
                FROM
                    department
                WHERE
                    name = '研发部'
            )
    );

SELECT *
FROM employee
WHERE (salary, manager_id) = (
        SELECT
            salary,
            manager_id
        FROM employee
        WHERE name = '张无忌'
    );

SELECT *
FROM employee
WHERE (job, department_id) IN (
        SELECT
            job,
            department_id
        FROM employee
        WHERE
            name IN('杨晓', '小昭')
    );

SELECT *
from (
        SELECT *
        FROM employee
        WHERE
            entry_date > '2002-01-01'
    ) e
    LEFT JOIN department d ON e.department_id = d.id;

select
    e.name,
    e.age,
    e.job,
    d.*
from employee e
    inner join department d on e.department_id = d.id;

insert into
    employee (
        id,
        name,
        age,
        job,
        salary,
        entry_date
    )
VALUES
(
        7,
        '张婷婷',
        41,
        '啦啦队',
        4000,
        '2008-04-11'
    )