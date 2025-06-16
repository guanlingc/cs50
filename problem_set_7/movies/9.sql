-- list names of allpeople who starred in 2004 movie by birth year
SELECT DISTINCT name FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies on stars.movie_id = movies.id
WHERE year = 2004
ORDER BY birth ASC;



