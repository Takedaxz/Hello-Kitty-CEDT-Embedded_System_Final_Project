// LED Toggle Functionality
const toggleSwitches = document.querySelectorAll('.toggle');

toggleSwitches.forEach(toggle => {
  toggle.addEventListener('click', () => {
    toggle.classList.toggle('active'); // Change toggle state
    const status = toggle.classList.contains('active') ? 'ON' : 'OFF';
    console.log(`LED is now: ${status}`); // Log status (can integrate with backend API here)
  });
});
const cards = document.querySelectorAll('.card');
    let draggedCard = null;

    cards.forEach(card => {
      card.addEventListener('dragstart', (e) => {
        draggedCard = e.target;
        setTimeout(() => {
          e.target.style.opacity = '0.5';
        }, 0);
      });

      card.addEventListener('dragend', (e) => {
        setTimeout(() => {
          draggedCard.style.opacity = '1';
          draggedCard = null;
        }, 0);
      });

      card.addEventListener('dragover', (e) => {
        e.preventDefault();
      });

      card.addEventListener('dragenter', (e) => {
        e.preventDefault();
        if (e.target.className === 'card') {
          e.target.style.backgroundColor = '#f1f1f1';
        }
      });

      card.addEventListener('dragleave', (e) => {
        e.target.style.backgroundColor = '';
      });

      card.addEventListener('drop', (e) => {
        e.preventDefault();
        if (e.target.className === 'card') {
          e.target.style.backgroundColor = '';
          if (draggedCard !== e.target) {
            const cardsList = [...document.querySelectorAll('.card')];
            const draggedIndex = cardsList.indexOf(draggedCard);
            const targetIndex = cardsList.indexOf(e.target);

            if (draggedIndex < targetIndex) {
              e.target.after(draggedCard);
            } else {
              e.target.before(draggedCard);
            }
          }
        }
      });
    });