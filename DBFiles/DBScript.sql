CREATE TABLE IF NOT EXISTS students
(
    id           SERIAL PRIMARY KEY,            -- Auto-incrementing integer for unique student ID
    student_name VARCHAR(255) NOT NULL,         -- Student's full name
    email        VARCHAR(255) UNIQUE NOT NULL,  -- Student's email address (unique)
    age          INT NOT NULL,                  -- Student's age
    stream       VARCHAR(255) NOT NULL          -- Academic stream or major of the student
    );

CREATE TABLE IF NOT EXISTS courses
(
    course_id    SERIAL PRIMARY KEY,            -- Unique ID for each course
    course_name  VARCHAR(255) NOT NULL,         -- Name of the course
    course_code  VARCHAR(10) UNIQUE NOT NULL    -- Short code for the course
    );

CREATE TABLE IF NOT EXISTS student_courses
(
    student_id INT NOT NULL,
    course_id  INT NOT NULL,
    FOREIGN KEY (student_id) REFERENCES students(id),
    FOREIGN KEY (course_id) REFERENCES courses(course_id),
    PRIMARY KEY (student_id, course_id)         -- Composite key to ensure unique pairs
    );