-- all movies in 2010
-- all ratings in desc order
-- same rating order by title
-- omit titles with no ratings

SELECT title, rating FROM movies
JOIN ratings
ON movies.id = ratings.movie_id
WHERE year = 2010
ORDER BY rating DESC,title ASC;



