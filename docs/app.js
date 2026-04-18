const PROJECTS = [
  {
    id: "digital-clock",
    name: "Digital Clock",
    status: "complete",
    folder: "digital_clock",
    description: "A 7-segment digital clock built with SDL3.",
    highlights: [
      "Draws six 7-segment digits (HH:MM:SS)",
      "Reads local system time every frame",
      "Renders colon separators between digit pairs"
    ],
    tech: ["C", "SDL3", "Realtime", "Rendering"],
    image: "imgs/digital-clock.png",
    dependencies: ["GCC", "SDL3"],
    controls: ["Close window: quit"],
    build: "gcc digital_clock/main.c digital_clock/clock.c -o digital_clock/digital_clock -lSDL3",
    run: "./digital_clock/digital_clock"
  },
  {
    id: "doom",
    name: "Doom Raycasting Engine",
    status: "in-progress",
    folder: "doom",
    description: "A 3D raycasting engine using DDA for efficient ray-grid traversal.",
    highlights: [
      "DDA ray traversal through a 2D map",
      "First-person scene rendering from grid data",
      "Smooth movement and directional rotation"
    ],
    tech: ["C", "SDL3", "Raycasting", "DDA"],
    image: "imgs/raycaster-game001.png",
    dependencies: ["GCC", "SDL3", "libm"],
    controls: [
      "W / Up: forward",
      "S / Down: backward",
      "Q / Left: rotate left",
      "E / Right: rotate right"
    ],
    build: "gcc doom/doom.c doom/kengine.c -o doom/doom -lSDL3 -lm",
    run: "./doom/doom"
  },
  {
    id: "fluid",
    name: "Fluid Simulation",
    status: "complete",
    folder: "fluid",
    description: "A simple grid-based falling-fluid simulation built with SDL3.",
    highlights: [
      "Fixed grid with fluid, wall, and empty cells",
      "Frame-by-frame fluid falling update",
      "Real-time grid painting with brush modes"
    ],
    tech: ["C", "SDL3", "Simulation", "Cellular Automata"],
    image: "imgs/fluid-v1.png",
    dependencies: ["GCC", "SDL3"],
    controls: [
      "Left mouse drag: paint selected cell",
      "F: fluid brush",
      "W: wall brush",
      "E: eraser",
      "C: clear grid",
      "Esc: quit"
    ],
    build: "gcc fluid/fluid.c -o fluid/fluid -lSDL3",
    run: "./fluid/fluid"
  },
  {
    id: "random-walk",
    name: "Random Walk",
    status: "complete",
    folder: "random_walk",
    description: "A multi-agent random walk visualizer built with SDL3.",
    highlights: [
      "Spawns multiple agents from screen center",
      "Random cardinal-direction step per update",
      "HSL-based color generation per agent"
    ],
    tech: ["C", "SDL3", "Procedural", "Visualization"],
    image: "imgs/random-walk-v1.png",
    dependencies: ["GCC", "SDL3", "libm"],
    controls: ["Run with optional agent count: ./random_walk/random_walk 20"],
    build: "gcc random_walk/random_walk.c -o random_walk/random_walk -lSDL3 -lm",
    run: "./random_walk/random_walk <num_of_agent>"
  },
  {
    id: "space-sim",
    name: "Space Simulation",
    status: "in-progress",
    folder: "space_sim",
    description: "Bodies, mass, and orbital behavior simulation playground.",
    highlights: [
      "Hierarchical orbits with parent-child relationships",
      "Delta-time trigonometric orbit updates",
      "Texture rendering with optional orbit paths"
    ],
    tech: ["C", "SDL3", "SDL3_image", "Physics", "Simulation"],
    image: "imgs/space-sim-v0.1.png",
    dependencies: ["GCC", "SDL3", "SDL3_image", "libm"],
    controls: [
      "Mouse wheel up: zoom in",
      "Mouse wheel down: zoom out",
      "Close window: exit"
    ],
    build: "gcc space_sim/space_sim.c space_sim/body.c -o space_sim/space_sim -lSDL3 -lSDL3_image -lm",
    run: "./space_sim/space_sim"
  }
];

const state = {
  activeTags: new Set(),
  search: "",
  sort: "name-asc",
  sidebarProjectId: null,
  tagsExpanded: false
};

const TAG_COLLAPSE_THRESHOLD = 10;

const projectsGrid = document.getElementById("projectsGrid");
const projectCount = document.getElementById("projectCount");
const searchInput = document.getElementById("searchInput");
const sortSelect = document.getElementById("sortSelect");
const tagFilters = document.getElementById("tagFilters");
const emptyState = document.getElementById("emptyState");
const cardTemplate = document.getElementById("projectCardTemplate");
const detailsSidebar = document.getElementById("detailsSidebar");
const sidebarBackdrop = document.getElementById("sidebarBackdrop");
const closeSidebar = document.getElementById("closeSidebar");
const sidebarContent = document.getElementById("sidebarContent");

function getStatusLabel(status) {
  if (status === "complete") return "Complete";
  if (status === "in-progress") return "In Progress";
  return "Planned";
}

function getStatusWeight(status) {
  if (status === "complete") return 0;
  if (status === "in-progress") return 1;
  return 2;
}

function applyTagCollapseState() {
  const extraTags = tagFilters.querySelectorAll(".tag[data-extra='true']");
  const toggle = tagFilters.querySelector(".tag-toggle");

  extraTags.forEach((button) => {
    const isActive = button.getAttribute("aria-pressed") === "true";
    button.hidden = !state.tagsExpanded && !isActive;
  });

  if (toggle) {
    toggle.textContent = state.tagsExpanded ? "Show less" : "Show more";
    toggle.setAttribute("aria-expanded", String(state.tagsExpanded));
  }
}

function buildTagFilters() {
  const tags = [...new Set(PROJECTS.flatMap((project) => project.tech))].sort((a, b) =>
    a.localeCompare(b)
  );

  const fragment = document.createDocumentFragment();
  tags.forEach((tag) => {
    const button = document.createElement("button");
    button.className = "tag";
    button.type = "button";
    button.textContent = tag;
    button.setAttribute("aria-pressed", "false");

    button.addEventListener("click", () => {
      if (state.activeTags.has(tag)) {
        state.activeTags.delete(tag);
      } else {
        state.activeTags.add(tag);
      }
      button.setAttribute("aria-pressed", String(state.activeTags.has(tag)));
      applyTagCollapseState();
      render();
    });

    fragment.appendChild(button);
  });

  if (tags.length > TAG_COLLAPSE_THRESHOLD) {
    const toggle = document.createElement("button");
    toggle.className = "tag tag-toggle";
    toggle.type = "button";
    toggle.textContent = "Show more";
    toggle.setAttribute("aria-expanded", "false");
    toggle.addEventListener("click", () => {
      state.tagsExpanded = !state.tagsExpanded;
      applyTagCollapseState();
    });

    fragment.appendChild(toggle);
  }

  tagFilters.replaceChildren(fragment);

  if (tags.length > TAG_COLLAPSE_THRESHOLD) {
    const tagButtons = tagFilters.querySelectorAll(".tag:not(.tag-toggle)");
    tagButtons.forEach((button, index) => {
      if (index >= TAG_COLLAPSE_THRESHOLD) {
        button.dataset.extra = "true";
      }
    });

    applyTagCollapseState();
  }
}

function getFilteredProjects() {
  const search = state.search.trim().toLowerCase();

  let filtered = PROJECTS.filter((project) => {
    const matchesSearch =
      !search ||
      project.name.toLowerCase().includes(search) ||
      project.description.toLowerCase().includes(search) ||
      project.tech.some((item) => item.toLowerCase().includes(search)) ||
      project.highlights.some((item) => item.toLowerCase().includes(search));

    const matchesTags =
      state.activeTags.size === 0 ||
      [...state.activeTags].every((tag) => project.tech.includes(tag));

    return matchesSearch && matchesTags;
  });

  filtered = filtered.sort((a, b) => {
    if (state.sort === "name-desc") return b.name.localeCompare(a.name);
    if (state.sort === "status") return getStatusWeight(a.status) - getStatusWeight(b.status);
    return a.name.localeCompare(b.name);
  });

  return filtered;
}

function createProjectCard(project) {
  const card = cardTemplate.content.firstElementChild.cloneNode(true);

  const image = card.querySelector(".project-card__image");
  image.src = project.image;
  image.alt = `${project.name} preview`;

  card.querySelector(".project-card__title").textContent = project.name;

  const status = card.querySelector(".project-card__status");
  status.textContent = getStatusLabel(project.status);
  status.dataset.status = project.status;

  card.querySelector(".project-card__description").textContent = project.description;

  const techList = card.querySelector(".project-card__tech");
  project.tech.forEach((tag) => {
    const item = document.createElement("li");
    item.textContent = tag;
    techList.appendChild(item);
  });

  const button = card.querySelector(".project-card__button");
  button.addEventListener("click", () => {
    openSidebar(project.id);
  });

  return card;
}

function renderCards(projects) {
  const fragment = document.createDocumentFragment();

  projects.forEach((project) => {
    fragment.appendChild(createProjectCard(project));
  });

  projectsGrid.replaceChildren(fragment);
  emptyState.hidden = projects.length !== 0;
}

function renderSidebar(projectId) {
  const project = PROJECTS.find((item) => item.id === projectId);

  if (!project) {
    sidebarContent.innerHTML = "<p>No details available.</p>";
    return;
  }

  const highlights = project.highlights.map((entry) => `<li>${entry}</li>`).join("");
  const controls = project.controls.map((entry) => `<li>${entry}</li>`).join("");
  const dependencies = project.dependencies.map((entry) => `<li>${entry}</li>`).join("");

  sidebarContent.innerHTML = `
    <h3>${project.name}</h3>
    <p>${project.description}</p>
    <p><strong>Status:</strong> ${getStatusLabel(project.status)}</p>
    <h4>Highlights</h4>
    <ul class="panel-list">${highlights}</ul>
    <h4>Stack</h4>
    <p>${project.tech.join(" - ")}</p>
    <h4>Controls</h4>
    <ul class="panel-list">${controls}</ul>
    <h4>Dependencies</h4>
    <ul class="panel-list">${dependencies}</ul>
    <h4>Build</h4>
    <p><code>${project.build}</code></p>
    <h4>Run</h4>
    <p><code>${project.run}</code></p>
    <div class="panel-links">
      <a href="https://github.com/kann4n/SDL_Projects/blob/main/${project.folder}/README.md" target="_blank" rel="noreferrer">Open README</a>
      <a href="https://github.com/kann4n/SDL_Projects/tree/main/${project.folder}" target="_blank" rel="noreferrer">Open Folder</a>
    </div>
  `;
}

function openSidebar(projectId) {
  state.sidebarProjectId = projectId;
  renderSidebar(projectId);
  detailsSidebar.classList.add("sidebar--open");
  detailsSidebar.setAttribute("aria-hidden", "false");
  sidebarBackdrop.hidden = false;
}

function closeDetailsSidebar() {
  detailsSidebar.classList.remove("sidebar--open");
  detailsSidebar.setAttribute("aria-hidden", "true");
  sidebarBackdrop.hidden = true;
}

function render() {
  const filtered = getFilteredProjects();
  renderCards(filtered);
  if (projectCount) projectCount.textContent = String(filtered.length);

  if (state.sidebarProjectId && !filtered.some((item) => item.id === state.sidebarProjectId)) {
    closeDetailsSidebar();
  }
}

searchInput.addEventListener("input", (event) => {
  state.search = event.target.value;
  render();
});

sortSelect.addEventListener("change", (event) => {
  state.sort = event.target.value;
  render();
});

closeSidebar.addEventListener("click", closeDetailsSidebar);
sidebarBackdrop.addEventListener("click", closeDetailsSidebar);
document.addEventListener("keydown", (event) => {
  if (event.key === "Escape") {
    closeDetailsSidebar();
  }
});

buildTagFilters();
render();
