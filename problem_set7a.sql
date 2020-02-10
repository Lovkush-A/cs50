SELECT title FROM movies WHERE (year == 2008);

SELECT birth FROM people WHERE name == "Emma Stone";

SELECT title FROM movies WHERE (year >= 2018) ORDER BY title;

SELECT COUNT(title) FROM
movies JOIN ratings ON movies.id = ratings.movie_id
WHERE (rating == 10);

SELECT title, year FROM movies WHERE title LIKE "Harry Potter%" ORDER BY year;

SELECT AVG(rating) FROM
movies JOIN ratings ON movies.id = ratings.movie_id
WHERE year == 2012;

SELECT title, rating FROM
movies JOIN ratings ON movies.id = ratings.movie_id
WHERE year == 2010 AND rating IS NOT NULL
ORDER BY rating DESC, title;

SELECT name FROM
movies JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE title == "Toy Story";

SELECT name FROM
movies JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE year == 2004
GROUP BY name
ORDER BY birth;

SELECT name FROM
movies JOIN ratings ON movies.id = ratings.movie_id
JOIN directors ON movies.id = directors.movie_id
JOIN people ON directors.person_id = people.id
WHERE rating >= 9.0
GROUP BY name;

SELECT title FROM
movies JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
JOIN ratings ON movies.id = ratings.movie_id
WHERE name = "Chadwick Boseman"
ORDER BY rating DESC
LIMIT 5;

SELECT title FROM
(SELECT title, COUNT(title) AS n FROM
movies JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE name == "Johnny Depp" OR name == "Helena Bonham Carter"
GROUP BY title)
WHERE n == 2;

SELECT peopleB.name FROM
    (SELECT movie_id AS A_movie_id FROM
    people peopleA JOIN stars starsA ON peopleA.id = starsA.person_id
    WHERE peopleA.name == 'Kevin Bacon' AND peopleA.birth == 1958)
JOIN stars starsB ON A_movie_id = starsB.movie_id
JOIN people peopleB ON peopleB.id = starsB.person_id
WHERE peopleB.name <> 'Kevin Bacon' OR peopleB.birth <> 1958
GROUP BY peopleB.name

