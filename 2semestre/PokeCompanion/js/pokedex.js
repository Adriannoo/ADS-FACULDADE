document.getElementById("searchButton").addEventListener("click", async () => {
    const query = document.getElementById("pokemonInput").value.toLowerCase().trim();
    if (!query) return alert("Digite um nome ou número!");

    const url = `https://pokeapi.co/api/v2/pokemon/${query}`;
    const response = await fetch(url);

    if (!response.ok) {
        document.getElementById("pokemonList").innerHTML = `<p>Pokémon não encontrado 😢</p>`;
        return;
    }

    const data = await response.json();
    renderPokemon(data);
});

function renderPokemon(pokemon) {
    const container = document.getElementById("pokemonList");
    container.innerHTML = `
        <div class="pokemon-card">
        <img src="${pokemon.sprites.other['official-artwork'].front_default}" alt="${pokemon.name}">
        <h3>${pokemon.name.toUpperCase()}</h3>
        <p><strong>#${pokemon.id}</strong></p>
        <p>Altura: ${(pokemon.height / 10).toFixed(1)} m</p>
        <p>Peso: ${(pokemon.weight / 10).toFixed(1)} kg</p>
        <p>Tipo: ${pokemon.types.map(t => t.type.name).join(', ')}</p>
        </div>
    `;
}
