patr-homework
=============
PATR Homeworks
<h3 align="center">Problema 2. Modelarea accesului la o statie de benzina</h3>
<p>Modelarea accesului la o statie de benzina
Clientii pot alimenta cu combustibil la o benzina in maniera self-service. Organizarea accesului
este urmatoarea:
</p>
	<ul>
		<li>
		daca o pompa este libera, clientul o poate folosi;
	</li>
		<li>
		daca nu este nici o pompa libera, clientul trebuie sa astepte eliberarea unei pompe;
	</li>
		<li>
		casierul va elibera pompa, semnalizand clientului ca poate alimenta cu combustibil;
	</li>
		<li>
		dupa ce a finalizat activitatea de alimentare cu combustibil, clientul va plati casierului suma
		corespunzatoare.
	</li>
</ul>
<p>
Sa se implementeze o aplicatie formata din mai multe taskuri, cu mecanismele specifice QNX,
care sa simuleze functionarea specificata. Se vor considera urmatoarele doua cazuri:
	<ol>
		<li>
		se considera ca statia de benzina are o singura pompa, un singur casier, si exista N clienti care
		incearca sa alimenteze;
	</li>
		<li>
		se considera ca statia de benzina are M pompe, un singur casier, si exista N clienti care
		incearca sa alimenteze.
	</li>
</ol>
</p>
	<ul>
		<li>
		<a href="https://docs.google.com/document/d/1_tC_zMdqW7gt5gbKVjFjHSELRajd6kXJ5KmmjQyHk4o/edit?usp=sharing" target="blank" >Documentation</a>
	</li>
</ul>
<br />
<br />
<br />
<h3 align="center">Problema 5. Gestionarea unui cont bancar</h3>
<p>
Un cont bancar poate fi utilizat de mai multe persoane (taskuri). Fiecare persoana poate
depozita sau retrage anumite sume de bani din contul comun.
Sa se implementeze o aplicatie formata din mai multe taskuri, cu mecanismele specifice QNX,
care sa simuleze functionarea specificata. Aplicatia multitasking va fi formata din urmatoarele tipuri
de taskuri:
</p>
	<ol>
	<li>task depunere: va depune o suma in contul comun (va face o cerere de depunere);</li>
		<li>task retragere: va incerca sa retraga o suma de bani din contul comun (va face o cerere de
	retragere);</li>
		<li>task server: va monitoriza situatia depunerilor / retragerilor in/din contul comun, astfel incat
		balanta (suma depunerilor minus suma retragerilor, la zi) sa nu fie negativa. Va afisa de
	asemenea balanta contului.</li>
</ol>

<ul>
		<li>
		<a href="https://docs.google.com/document/d/1U4NeSNT98vvow49kn-eiv0dv3gdUPRsveOCw1B8fIPs/edit?usp=sharing" target="blank" >Documentation</a>
	</li>
</ul>