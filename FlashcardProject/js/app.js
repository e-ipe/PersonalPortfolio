const term = document.querySelector('.term');
const definition = document.querySelector('.definition');
const checkButton = document.querySelector('.check');
const nextButton = document.querySelector('.next');

let words = {
    Hello: "Hola",
    Goodbye: "Adios",
    Water: "Agua",
    Please: 'Por favor',
    Good: "Bien"
};

data = Object.entries(words); /* Translates into an array -> key: value */

function getRandomWord() {
    randomTerm = data[Math.floor(Math.random() * data.length)]
    term.innerHTML = `<h3>${randomTerm[0]}</h3>` /* Shows the term */
    definition.innerHTML = `<h3>${randomTerm[1]}</h3>` /* Shows the definition */
}
    
checkButton.addEventListener('click', function() {
    console.log("Check button clicked");
    definition.style.display = 'block'; /* Displays the definition only when the button is clicked */
});
    
nextButton.addEventListener('click', function() {
    console.log("Next button clicked");
    /* Calls the next word and hides the definition */
    getRandomWord();
    definition.style.display = 'none';
});
