const POSTS_ENDPOINT = "/database/posts.py";

function readCookie(name) {
	const prefix = `${name}=`;
	const cookie = document.cookie
		.split("; ")
		.find((entry) => entry.startsWith(prefix));

	return cookie ? decodeURIComponent(cookie.slice(prefix.length)) : "";
}

function getAvatarSrc(username) {
	if (!username) {
		return "";
	}

	const version = readCookie("papertrail_avatar_version");
	const cacheBuster = version ? `?v=${encodeURIComponent(version)}` : "";
	return `/assets/profile_pictures/${username}.png${cacheBuster}`;
}

async function fetchPosts() {
const response = await fetch(POSTS_ENDPOINT, { method: "GET", cache: "no-store" });
if (!response.ok) {
throw new Error("Unable to load posts.");
}

const posts = await response.json();
return Array.isArray(posts) ? posts : [];
}

async function persistPosts(payload, method, postId = "") {
const endpoint = method === "DELETE" && postId
? `${POSTS_ENDPOINT}?id=${encodeURIComponent(postId)}`
: POSTS_ENDPOINT;
const response = await fetch(endpoint, {
method,
headers: {
"Content-Type": "application/json",
},
body: method === "DELETE" ? undefined : JSON.stringify(payload),
});

if (!response.ok) {
throw new Error("Request failed.");
}

return response.json();
}

function initialsFromName(name) {
return name
.split(/\s+/)
.filter(Boolean)
.slice(0, 2)
.map((part) => part[0].toUpperCase())
.join("") || "?";
}

function escapeHtml(value) {
	return String(value)
		.replace(/&/g, "&amp;")
		.replace(/</g, "&lt;")
		.replace(/>/g, "&gt;")
		.replace(/\"/g, "&quot;")
		.replace(/'/g, "&#39;");
}

function avatarMarkup(post) {
	if (!post.avatar) {
		return `<div class="author-avatar ${post.author && post.author.length % 2 === 0 ? "alt" : ""}">${initialsFromName(post.author || "Anonymous")}</div>`;
	}

	const avatarSrc = getAvatarSrc(post.avatar);
	const initials = initialsFromName(post.author || "Anonymous");
	return `
		<div class="author-avatar image-avatar ${post.author && post.author.length % 2 === 0 ? "alt" : ""}">
			<img src="${escapeHtml(avatarSrc)}" alt="${escapeHtml(post.author || "Anonymous")} profile picture" onerror="this.remove();this.parentElement.textContent='${escapeHtml(initials)}';this.parentElement.classList.remove('image-avatar');">
		</div>
	`;
}

function formatDate(value) {
return new Date(value).toLocaleDateString("en-US", {
month: "long",
day: "numeric",
year: "numeric",
});
}

function getProfileNameFallback() {
const name = document.cookie.match(/(?:^|; )papertrail_name=([^;]+)/)?.[1] || "";
const surname = document.cookie.match(/(?:^|; )papertrail_surname=([^;]+)/)?.[1] || "";
return decodeURIComponent([name, surname].filter(Boolean).join(" ")).trim();
}

function createPostElement(post, onLike, onEdit, onDelete) {
const article = document.createElement("article");
article.className = "post card";

const authorName = post.author || "Anonymous";
article.innerHTML = `
<div class="post-meta">
<div class="author">
				${avatarMarkup(post)}
<div>
<p class="author-name">${authorName}</p>
<p class="muted">${formatDate(post.createdAt)}</p>
</div>
</div>
<button class="like-button" type="button">♡ Like (<span>${post.likes}</span>)</button>
</div>
<h3>${post.title}</h3>
<p>${post.body}</p>
<div class="post-actions">
<button class="post-action" type="button">Edit</button>
<button class="post-action" type="button">Delete</button>
</div>
`;

const likeButton = article.querySelector(".like-button");
const editButton = article.querySelectorAll(".post-action")[0];
const deleteButton = article.querySelectorAll(".post-action")[1];

likeButton.addEventListener("click", () => onLike(post.id));
editButton.addEventListener("click", () => onEdit(post.id));
deleteButton.addEventListener("click", () => onDelete(post.id));

return article;
}

document.addEventListener("DOMContentLoaded", () => {
const postsList = document.getElementById("posts-list");
const emptyState = document.getElementById("empty-state");
const form = document.getElementById("post-form");
const postIdField = document.getElementById("post-id");
const authorField = document.getElementById("post-author");
const actionField = document.getElementById("post-action");
	const avatarField = document.getElementById("post-avatar");
	const avatarVersionField = document.getElementById("post-avatar-version");
const titleField = document.getElementById("post-title");
const bodyField = document.getElementById("post-body");
const submitButton = document.getElementById("post-submit");
const cancelButton = document.getElementById("post-cancel");

if (!postsList || !emptyState || !form) {
return;
}

let posts = [];

function resetEditor() {
postIdField.value = "";
authorField.value = "";
		avatarField.value = "";
		avatarVersionField.value = "";
actionField.value = "create";
titleField.value = "";
bodyField.value = "";
submitButton.textContent = "Publish post";
cancelButton.hidden = true;
}

async function refreshPosts() {
try {
posts = await fetchPosts();
} catch {
posts = [];
}
renderPosts();
}

function renderPosts() {
postsList.innerHTML = "";
emptyState.hidden = posts.length !== 0;

posts
.slice()
.sort((left, right) => new Date(right.createdAt) - new Date(left.createdAt))
.forEach((post) => {
postsList.appendChild(
createPostElement(
post,
(postId) => {
const updatedPost = posts.find((item) => item.id === postId);
if (!updatedPost) {
return;
}

const payload = {
action: "like",
post: { ...updatedPost, likes: (updatedPost.likes || 0) + 1 },
};
persistPosts(payload, "POST")
.then((updatedPosts) => {
posts = updatedPosts;
renderPosts();
})
.catch(() => alert("Unable to like post right now."));
},
(postId) => {
const postToEdit = posts.find((item) => item.id === postId);
if (!postToEdit) {
return;
}

postIdField.value = postToEdit.id;
authorField.value = postToEdit.author || "";
titleField.value = postToEdit.title;
bodyField.value = postToEdit.body;
submitButton.textContent = "Update post";
cancelButton.hidden = false;
},
(postId) => {
posts = posts.filter((item) => item.id !== postId);
persistPosts({ id: postId }, "DELETE", postId)
.then((updatedPosts) => {
posts = updatedPosts;
if (postIdField.value === String(postId)) {
resetEditor();
}
renderPosts();
})
.catch(() => alert("Unable to delete post right now."));
}
)
);
});
}

form.addEventListener("submit", (event) => {
const author = getProfileNameFallback() || "Anonymous";
		const avatar = readCookie("papertrail_username");
		const avatarVersion = readCookie("papertrail_avatar_version");
const title = titleField.value.trim();
const body = bodyField.value.trim();
const editingPostId = postIdField.value;

authorField.value = author;
		avatarField.value = avatar;
		avatarVersionField.value = avatarVersion;
actionField.value = editingPostId ? "update" : "create";

if (!title || !body) {
event.preventDefault();
return;
}

if (editingPostId) {
event.preventDefault();
const existingPost = posts.find((post) => post.id === editingPostId);
const payload = {
action: "update",
post: {
...(existingPost || {}),
id: editingPostId,
author,
					avatar,
					avatarVersion,
title,
body,
likes: existingPost ? existingPost.likes : 0,
},
};
persistPosts(payload, "POST", editingPostId)
.then((updatedPosts) => {
posts = updatedPosts;
resetEditor();
renderPosts();
})
.catch(() => alert("Unable to update post right now."));
}
});

cancelButton.addEventListener("click", () => {
resetEditor();
});

refreshPosts();
});
