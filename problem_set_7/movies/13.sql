--list all names who starred movie which kevin bacon also did
-- p1 is people table 1
-- match people with stars and additional stars table
-- then join stars with people
-- filter people to be kevin bacon and birth year
-- remove kevin bacon from results.
-- people1 > star1 >star2>people2
-- 1958 KB > person_id > movie_id > people > return names
SELECT DISTINCT p2.name FROM people AS p1
JOIN stars AS s1 ON p1.id = s1.person_id
JOIN stars AS s2 ON s1.movie_id = s2.movie_id
JOIN people AS p2 ON s2.person_id = p2.id
WHERE p1.name = 'Kevin Bacon' AND p1.birth = 1958
AND p2.id != p1.id;

