-- Active: 1699918881937@@127.0.0.1@3306@mysql_learn

/* 1 */

select
    e.name,
    e.age,
    e.job,
    d.*
from employee e
    inner join department d on e.department_id = d.id;

/* 2 */

select
    e.name,
    e.age,
    e.job,
    d.*
from (
        select
            name,
            age,
            job,
            department_id
        from employee
        where age < 30
    ) e
    inner join department d on e.department_id = d.id;

/* 3 */

select d.id, d.name, count(*)
from department d
    join employee e on d.id = e.department_id
GROUP BY d.id;

/* 4 */

select
    e.name,
    e.age,
    e.job,
    d.*
from (
        select
            name,
            age,
            job,
            department_id
        from employee
        where age > 40
    ) e
    left join department d on e.department_id = d.id;

/* 5 */

SELECT
    *,
    case
        when salary >= 15000 then '顶级'
        when salary >= 10000 then '高级'
        when salary >= 8000 then '中级'
        when salary >= 5000 then '初级'
        else '低级'
    end '工资等级'
from employee;

/* 6 */

SELECT
    *,
    case
        when salary >= 15000 then '顶级'
        when salary >= 10000 then '高级'
        when salary >= 8000 then '中级'
        when salary >= 5000 then '初级'
        else '低级'
    end '工资等级'
FROM employee
WHERE department_id = (
        SELECT id
        from department
        WHERE name = '研发部'
    );

/* 7 */

SELECT AVG(salary)
FROM employee
WHERE department_id = (
        SELECT id
        from department
        WHERE name = '研发部'
    );

/* 8 */

SELECT *
FROM employee
WHERE salary > (
        SELECT salary
        FROM employee
        WHERE name = '韦一笑'
    );

/* 9 */

SELECT *
FROM employee
WHERE salary > (
        SELECT avg(salary)
        FROM employee
    );

/* 10 */

SELECT *
FROM employee e
    inner join (
        SELECT
            department_id,
            AVG(salary) avg_salary
        FROM employee
        GROUP BY
            department_id
    ) d on e.department_id = d.department_id
    and e.salary < d.avg_salary;