-- list names of all people who starred in toy story
-- output 1 col of names

SELECT name FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people on people.id = stars.person_id
WHERE title = 'Toy Story';
