-- all movies and their ratings in 2012
-- desc order
-- output table single col and single row

SELECT AVG(rating) FROM ratings
JOIN movies
ON movies.id = ratings.movie_id
WHERE year = 2012;
