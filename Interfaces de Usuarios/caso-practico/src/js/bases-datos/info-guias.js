// ======================================================
// PARTE 1 — GUÍAS 1 A 5 (TRADUCIDAS)
// ======================================================

const guias = [

    // ==================================================
    // GUÍA 1: EGIPTO — CRUCERO POR EL NILO
    // ==================================================
    {
        id: 1,
        titulo: {
            es: "Un crucero cultural por el Nilo: guía completa",
            en: "A cultural cruise on the Nile: complete guide",
            it: "Una crociera culturale sul Nilo: guida completa"
        },
        categoria: {
            es: "Viajes culturales",
            en: "Cultural trips",
            it: "Viaggi culturali"
        },
        destino: "Egipto",
        tipo: {
            es: "Guía práctica",
            en: "Practical guide",
            it: "Guida pratica"
        },
        imagen: "https://images.unsplash.com/photo-1548679057-37a544ca945e?w=800",
        lectura: "15 min",
        valoracion: "4.9",
        desc: {
            es: "Viajar por el Nilo es una de las formas más completas y cómodas de descubrir el Antiguo Egipto. En esta guía te explicamos cómo funciona un crucero fluvial, qué paradas incluye, cuánto cuesta realmente, qué excursiones merecen la pena y cuál es la mejor época para hacerlo sin sufrir el calor extremo.",
            en: "Traveling along the Nile is one of the most complete and comfortable ways to discover Ancient Egypt. This guide explains how river cruises work, what stops are included, real costs, worthwhile excursions, and the best time to go without extreme heat.",
            it: "Viaggiare lungo il Nilo è uno dei modi più completi e comodi per scoprire l'Antico Egitto. Questa guida spiega come funzionano le crociere fluviali, le tappe incluse, i costi reali e il periodo migliore per evitare il caldo estremo."
        },
        tags: ["Historia", "Barco", "Cultura"],

        detalle: {
            subtitulo: {
                es: "Itinerario cultural de 7 días navegando entre templos y pueblos ribereños.",
                en: "7-day cultural itinerary sailing between temples and riverside villages.",
                it: "Itinerario culturale di 7 giorni tra templi e villaggi sul fiume."
            },
            autor: "Egypt Tourism Board",
            intro: {
                es: "<strong>El Nilo es el corazón de Egipto.</strong> Este itinerario está pensado para mochileros que quieren una experiencia cultural intensa sin complicaciones logísticas.",
                en: "<strong>The Nile is the heart of Egypt.</strong> This itinerary is designed for backpackers seeking an intense cultural experience without logistical hassles.",
                it: "<strong>Il Nilo è il cuore dell'Egitto.</strong> Questo itinerario è pensato per backpacker che vogliono un’esperienza culturale intensa senza complicazioni."
            },

            itinerario: [
                {
                    dia: 1,
                    titulo: { es: "Luxor y embarque", en: "Luxor & boarding", it: "Luxor e imbarco" },
                    desc: {
                        es: "Llegada a Luxor, check-in y visita nocturna al templo.",
                        en: "Arrival in Luxor, check-in and night visit to the temple.",
                        it: "Arrivo a Luxor, check-in e visita notturna al tempio."
                    }
                },
                {
                    dia: 2,
                    titulo: { es: "Valle de los Reyes", en: "Valley of the Kings", it: "Valle dei Re" },
                    desc: {
                        es: "Tumbas reales y templo de Karnak.",
                        en: "Royal tombs and Karnak temple.",
                        it: "Tombe reali e tempio di Karnak."
                    }
                },
                {
                    dia: 3,
                    titulo: { es: "Edfu", en: "Edfu", it: "Edfu" },
                    desc: {
                        es: "Templo de Horus y navegación.",
                        en: "Temple of Horus and sailing.",
                        it: "Tempio di Horus e navigazione."
                    }
                },
                {
                    dia: 4,
                    titulo: { es: "Kom Ombo", en: "Kom Ombo", it: "Kom Ombo" },
                    desc: {
                        es: "Templo doble y museo del cocodrilo.",
                        en: "Double temple and crocodile museum.",
                        it: "Tempio doppio e museo del coccodrillo."
                    }
                }
            ],

            consejos: {
                es: [
                    "Lleva billetes pequeños para propinas.",
                    "Evita el verano por el calor extremo.",
                    "Contrata excursiones oficiales."
                ],
                en: [
                    "Carry small bills for tips.",
                    "Avoid summer due to extreme heat.",
                    "Book official excursions."
                ],
                it: [
                    "Porta banconote piccole per le mance.",
                    "Evita l’estate per il caldo estremo.",
                    "Prenota escursioni ufficiali."
                ]
            },

            maleta: {
                clima: {
                    es: "Oct-Abr: 15–25°C | May-Sep: >35°C",
                    en: "Oct-Apr: 15–25°C | May-Sep: >35°C",
                    it: "Ott-Apr: 15–25°C | Mag-Set: >35°C"
                },
                items: {
                    es: ["Ropa transpirable", "Sombrero", "Protector solar", "Calzado cerrado"],
                    en: ["Breathable clothing", "Hat", "Sunscreen", "Closed shoes"],
                    it: ["Abbigliamento traspirante", "Cappello", "Crema solare", "Scarpe chiuse"]
                }
            },

            sidebar: {
                duracion: { es: "7 días / 6 noches", en: "7 days / 6 nights", it: "7 giorni / 6 notti" },
                epoca: { es: "Octubre - Abril", en: "October - April", it: "Ottobre - Aprile" },
                presupuesto: "80–120 € / día",
                intensidad: { es: "Media", en: "Medium", it: "Media" }
            }
        }
    },

    // ==================================================
    // GUÍA 2: EGIPTO — GIZA POR LIBRE
    // ==================================================
    {
        id: 2,
        titulo: {
            es: "Pirámides de Giza por libre y sin estafas",
            en: "Giza pyramids independently and scam-free",
            it: "Piramidi di Giza in autonomia e senza truffe"
        },
        categoria: {
            es: "Presupuesto bajo",
            en: "Low budget",
            it: "Basso budget"
        },
        destino: "Egipto",
        tipo: {
            es: "Consejos",
            en: "Tips",
            it: "Consigli"
        },
        imagen: "https://images.unsplash.com/photo-1503177119275-0aa32b3a9368?w=800",
        lectura: "10 min",
        valoracion: "4.8",
        desc: {
            es: "Visitar las pirámides de Giza por tu cuenta es totalmente posible y mucho más barato de lo que parece. Te explicamos cómo llegar sin tours, los precios oficiales y cómo evitar estafas.",
            en: "Visiting the Giza pyramids on your own is totally possible and much cheaper than it seems. Learn how to get there, official prices, and how to avoid scams.",
            it: "Visitare le piramidi di Giza da soli è possibile ed economico. Scopri come arrivare, i prezzi ufficiali e come evitare le truffe."
        },
        tags: ["Ahorro", "Historia"],

        detalle: {
            subtitulo: {
                es: "La forma más económica de visitar una maravilla del mundo.",
                en: "The cheapest way to visit a world wonder.",
                it: "Il modo più economico per visitare una meraviglia del mondo."
            },
            autor: "Mochileros Staff",
            intro: {
                es: "Visitar Giza por libre es totalmente posible si sabes cómo moverte.",
                en: "Visiting Giza independently is easy if you know how to get around.",
                it: "Visitare Giza in autonomia è semplice se sai come muoverti."
            },

            itinerario: [
                {
                    dia: 1,
                    titulo: { es: "Complejo de Giza", en: "Giza complex", it: "Complesso di Giza" },
                    desc: {
                        es: "Keops, Kefrén, Micerinos y la Esfinge.",
                        en: "Khufu, Khafre, Menkaure and the Sphinx.",
                        it: "Cheope, Chefren, Micerino e la Sfinge."
                    }
                }
            ],

            consejos: {
                es: ["Usa Uber", "Compra entradas oficiales", "No aceptes ayuda no solicitada"],
                en: ["Use Uber", "Buy official tickets", "Do not accept unsolicited help"],
                it: ["Usa Uber", "Compra biglietti ufficiali", "Non accettare aiuti non richiesti"]
            },

            maleta: {
                clima: {
                    es: "Caluroso y seco",
                    en: "Hot and dry",
                    it: "Caldo e secco"
                },
                items: {
                    es: ["Agua", "Gorra", "Zapatillas cómodas"],
                    en: ["Water", "Cap", "Comfortable shoes"],
                    it: ["Acqua", "Cappello", "Scarpe comode"]
                }
            },

            sidebar: {
                duracion: { es: "1 día", en: "1 day", it: "1 giorno" },
                epoca: { es: "Todo el año", en: "All year", it: "Tutto l’anno" },
                presupuesto: "20–30 €",
                intensidad: { es: "Baja", en: "Low", it: "Bassa" }
            }
        }
    },

    // --- GUÍA 3: ROMA ---
    {
        id: 3,
        titulo: {
            es: "Roma en 3 días: guía cultural completa",
            en: "Rome in 3 days: complete cultural guide",
            it: "Roma in 3 giorni: guida culturale completa"
        },
        categoria: {
            es: "Viajes culturales",
            en: "Cultural trips",
            it: "Viaggi culturali"
        },
        destino: "Italia",
        tipo: {
            es: "Ruta cultural",
            en: "Cultural route",
            it: "Percorso culturale"
        },
        imagen: "https://images.unsplash.com/photo-1549893077-16f0d5b27b36?w=800",
        lectura: "12 min",
        valoracion: "4.8",
        desc: {
            es: "Descubre los monumentos, plazas y museos imprescindibles de Roma en un itinerario optimizado de 3 días.",
            en: "Discover Rome's must-see monuments, squares, and museums in a 3-day optimized itinerary.",
            it: "Scopri i monumenti, piazze e musei imperdibili di Roma in un itinerario ottimizzato di 3 giorni."
        },
        tags: ["Historia", "Italia", "Cultura"],

        detalle: {
            subtitulo: {
                es: "Roma, un museo al aire libre.",
                en: "Rome, an open-air museum.",
                it: "Roma, un museo a cielo aperto."
            },
            autor: "Italia Travel",
            intro: {
                es: "Una ciudad para caminar y empaparse de historia y arte.",
                en: "A city to walk and soak up history and art.",
                it: "Una città da percorrere a piedi per immergersi nella storia e nell'arte."
            },

            itinerario: [
                { dia: 1, titulo: { es: "Centro histórico", en: "Historic center", it: "Centro storico" }, desc: { es: "Plazas, fuentes y monumentos.", en: "Squares, fountains and monuments.", it: "Piazze, fontane e monumenti." } },
                { dia: 2, titulo: { es: "Vaticano", en: "Vatican", it: "Vaticano" }, desc: { es: "Museos y Basílica de San Pedro.", en: "Museums and St. Peter's Basilica.", it: "Musei e Basilica di San Pietro." } },
                { dia: 3, titulo: { es: "Trastevere y colinas", en: "Trastevere and hills", it: "Trastevere e colline" }, desc: { es: "Calles con encanto y vistas panorámicas.", en: "Charming streets and panoramic views.", it: "Strade suggestive e vedute panoramiche." } }
            ],

            consejos: {
                es: ["Compra entradas online", "Usa calzado cómodo", "Evita horarios de máximo turismo"],
                en: ["Buy tickets online", "Wear comfortable shoes", "Avoid peak tourist hours"],
                it: ["Acquista i biglietti online", "Indossa scarpe comode", "Evita gli orari di punta"]
            },

            maleta: {
                clima: { es: "Templado", en: "Mild", it: "Mite" },
                items: {
                    es: ["Ropa cómoda", "Zapatos para caminar", "Sombrero"],
                    en: ["Comfortable clothes", "Walking shoes", "Hat"],
                    it: ["Abbigliamento comodo", "Scarpe da passeggio", "Cappello"]
                }
            },

            sidebar: {
                duracion: { es: "3 días", en: "3 days", it: "3 giorni" },
                epoca: { es: "Abr-Oct", en: "Apr-Oct", it: "Apr-Ott" },
                presupuesto: "50–80 €/día",
                intensidad: { es: "Media", en: "Medium", it: "Media" }
            }
        }
    },

    // --- GUÍA 4: NUEVA YORK ---
    {
        id: 4,
        titulo: {
            es: "Nueva York en 5 días: guía cultural y urbana",
            en: "New York in 5 days: cultural and urban guide",
            it: "New York in 5 giorni: guida culturale e urbana"
        },
        categoria: { es: "Viajes culturales", en: "Cultural trips", it: "Viaggi culturali" },
        destino: "EE. UU.",
        tipo: { es: "Ruta urbana", en: "Urban route", it: "Percorso urbano" },
        imagen: "https://images.unsplash.com/photo-1508051123996-69f8caf4891e?w=800",
        lectura: "15 min",
        valoracion: "4.7",
        desc: {
            es: "Recorre los barrios, museos y lugares emblemáticos de Nueva York en un itinerario práctico de 5 días.",
            en: "Explore New York's neighborhoods, museums, and landmarks in a practical 5-day itinerary.",
            it: "Esplora quartieri, musei e luoghi emblematici di New York in un itinerario pratico di 5 giorni."
        },
        tags: ["EEUU", "Cultura", "Ciudad"],

        detalle: {
            subtitulo: { es: "La Gran Manzana en 5 días.", en: "The Big Apple in 5 days.", it: "La Grande Mela in 5 giorni." },
            autor: "NY Travel",
            intro: { es: "Una ciudad de contrastes y cultura vibrante.", en: "A city of contrasts and vibrant culture.", it: "Una città di contrasti e cultura vibrante." },

            itinerario: [
                { dia: 1, titulo: { es: "Manhattan centro", en: "Midtown Manhattan", it: "Midtown Manhattan" }, desc: { es: "Times Square y Rockefeller.", en: "Times Square and Rockefeller.", it: "Times Square e Rockefeller." } },
                { dia: 2, titulo: { es: "Museos y Central Park", en: "Museums and Central Park", it: "Musei e Central Park" }, desc: { es: "Metropolitan y Natural History.", en: "Metropolitan and Natural History.", it: "Metropolitan e Natural History." } },
                { dia: 3, titulo: { es: "Brooklyn y puentes", en: "Brooklyn and bridges", it: "Brooklyn e ponti" }, desc: { es: "Paseo y vistas icónicas.", en: "Walk and iconic views.", it: "Passeggiata e viste iconiche." } },
                { dia: 4, titulo: { es: "Harlem y cultura", en: "Harlem and culture", it: "Harlem e cultura" }, desc: { es: "Música y gastronomía.", en: "Music and food.", it: "Musica e gastronomia." } },
                { dia: 5, titulo: { es: "Estatua de la Libertad", en: "Statue of Liberty", it: "Statua della Libertà" }, desc: { es: "Icono y recorrido en ferry.", en: "Icon and ferry tour.", it: "Icona e tour in traghetto." } }
            ],

            consejos: {
                es: ["Compra metro card", "Reserva museos online", "Camina mucho"],
                en: ["Buy metro card", "Book museums online", "Walk a lot"],
                it: ["Compra la metro card", "Prenota i musei online", "Cammina molto"]
            },

            maleta: {
                clima: { es: "Variable", en: "Variable", it: "Variabile" },
                items: {
                    es: ["Ropa cómoda", "Zapatillas", "Abrigo ligero"],
                    en: ["Comfortable clothes", "Sneakers", "Light coat"],
                    it: ["Abbigliamento comodo", "Scarpe da ginnastica", "Cappotto leggero"]
                }
            },

            sidebar: {
                duracion: { es: "5 días", en: "5 days", it: "5 giorni" },
                epoca: { es: "Abr-Oct", en: "Apr-Oct", it: "Apr-Ott" },
                presupuesto: "80–120 €/día",
                intensidad: { es: "Alta", en: "High", it: "Alta" }
            }
        }
    },

    // --- GUÍA 5: LONDRES ---
    {
        id: 5,
        titulo: {
            es: "Londres en 4 días: cultura y vida urbana",
            en: "London in 4 days: culture and urban life",
            it: "Londra in 4 giorni: cultura e vita urbana"
        },
        categoria: { es: "Viajes culturales", en: "Cultural trips", it: "Viaggi culturali" },
        destino: "Reino Unido",
        tipo: { es: "Ruta cultural", en: "Cultural route", it: "Percorso culturale" },
        imagen: "https://images.unsplash.com/photo-1528909514045-2fa4ac7a08ba?w=800",
        lectura: "11 min",
        valoracion: "4.7",
        desc: {
            es: "Explora los puntos icónicos de Londres, museos y barrios con esta ruta de 4 días.",
            en: "Explore London's iconic spots, museums, and neighborhoods in this 4-day route.",
            it: "Esplora i luoghi iconici di Londra, musei e quartieri in questo percorso di 4 giorni."
        },
        tags: ["Reino Unido", "Cultura", "Ciudad"],

        detalle: {
            subtitulo: { es: "Londres, historia y modernidad.", en: "London, history and modernity.", it: "Londra, storia e modernità." },
            autor: "UK Travel",
            intro: { es: "Una ciudad diversa que combina cultura, historia y vida moderna.", en: "A diverse city combining culture, history, and modern life.", it: "Una città diversa che combina cultura, storia e vita moderna." },

            itinerario: [
                { dia: 1, titulo: { es: "Westminster", en: "Westminster", it: "Westminster" }, desc: { es: "Big Ben y Abadía.", en: "Big Ben and Abbey.", it: "Big Ben e Abbazia." } },
                { dia: 2, titulo: { es: "Museos", en: "Museums", it: "Musei" }, desc: { es: "British Museum y National Gallery.", en: "British Museum and National Gallery.", it: "British Museum e National Gallery." } },
                { dia: 3, titulo: { es: "Covent Garden y Soho", en: "Covent Garden and Soho", it: "Covent Garden e Soho" }, desc: { es: "Barrio y compras.", en: "Neighborhood and shopping.", it: "Quartiere e shopping." } },
                { dia: 4, titulo: { es: "Torre de Londres", en: "Tower of London", it: "Torre di Londra" }, desc: { es: "Historia y vistas del río.", en: "History and river views.", it: "Storia e vedute sul fiume." } }
            ],

            consejos: {
                es: ["Compra tickets online", "Usa transporte público", "Camina para descubrir barrios"],
                en: ["Buy tickets online", "Use public transport", "Walk to discover neighborhoods"],
                it: ["Acquista biglietti online", "Usa trasporto pubblico", "Cammina per scoprire i quartieri"]
            },

            maleta: {
                clima: { es: "Templado", en: "Mild", it: "Mite" },
                items: {
                    es: ["Paraguas", "Ropa cómoda", "Zapatos cerrados"],
                    en: ["Umbrella", "Comfortable clothes", "Closed shoes"],
                    it: ["Ombrello", "Abbigliamento comodo", "Scarpe chiuse"]
                }
            },

            sidebar: {
                duracion: { es: "4 días", en: "4 days", it: "4 giorni" },
                epoca: { es: "Abr-Oct", en: "Apr-Oct", it: "Apr-Ott" },
                presupuesto: "70–100 €/día",
                intensidad: { es: "Media", en: "Medium", it: "Media" }
            }
        }
    },

    // --- GUÍA 6: TOKIO ---
    {
        id: 6,
        titulo: {
            es: "Tokio en 5 días: cultura y modernidad",
            en: "Tokyo in 5 days: culture and modernity",
            it: "Tokyo in 5 giorni: cultura e modernità"
        },
        categoria: { es: "Viajes culturales", en: "Cultural trips", it: "Viaggi culturali" },
        destino: "Japón",
        tipo: { es: "Ruta urbana", en: "Urban route", it: "Percorso urbano" },
        imagen: "https://images.unsplash.com/photo-1518684079-6a8fba0eb1db?w=800",
        lectura: "14 min",
        valoracion: "4.8",
        desc: {
            es: "Explora templos, barrios y cultura pop de Tokio en un itinerario práctico de 5 días.",
            en: "Explore temples, neighborhoods, and pop culture in Tokyo in a practical 5-day itinerary.",
            it: "Esplora templi, quartieri e cultura pop a Tokyo in un itinerario pratico di 5 giorni."
        },
        tags: ["Japón", "Cultura", "Ciudad"],

        detalle: {
            subtitulo: { es: "Tokio, tradición y vanguardia.", en: "Tokyo, tradition and avant-garde.", it: "Tokyo, tradizione e avanguardia." },
            autor: "Japan Travel",
            intro: { es: "Una ciudad donde lo tradicional y lo moderno conviven.", en: "A city where tradition and modernity coexist.", it: "Una città dove tradizione e modernità convivono." },

            itinerario: [
                { dia: 1, titulo: { es: "Shibuya y Shinjuku", en: "Shibuya and Shinjuku", it: "Shibuya e Shinjuku" }, desc: { es: "Cruces, rascacielos y vida urbana.", en: "Crossings, skyscrapers and urban life.", it: "Incroci, grattacieli e vita urbana." } },
                { dia: 2, titulo: { es: "Templos de Asakusa", en: "Asakusa temples", it: "Templi di Asakusa" }, desc: { es: "Sensō-ji y calles tradicionales.", en: "Sensō-ji and traditional streets.", it: "Sensō-ji e vie tradizionali." } },
                { dia: 3, titulo: { es: "Akihabara", en: "Akihabara", it: "Akihabara" }, desc: { es: "Electrónica y cultura pop.", en: "Electronics and pop culture.", it: "Elettronica e cultura pop." } },
                { dia: 4, titulo: { es: "Parques y jardines", en: "Parks and gardens", it: "Parchi e giardini" }, desc: { es: "Relajación y naturaleza en la ciudad.", en: "Relaxation and nature in the city.", it: "Relax e natura in città." } },
                { dia: 5, titulo: { es: "Mercados y gastronomía", en: "Markets and gastronomy", it: "Mercati e gastronomia" }, desc: { es: "Comida típica y compras.", en: "Typical food and shopping.", it: "Cibo tipico e shopping." } }
            ],

            consejos: {
                es: ["Compra metro pasmo", "Planifica rutas", "Aprende frases básicas"],
                en: ["Buy Pasmo metro card", "Plan routes", "Learn basic phrases"],
                it: ["Compra la Pasmo card", "Pianifica itinerari", "Impara frasi base"]
            },

            maleta: {
                clima: { es: "Variable", en: "Variable", it: "Variabile" },
                items: {
                    es: ["Ropa ligera y cómoda", "Zapatos cómodos", "Paraguas compacto"],
                    en: ["Light and comfortable clothes", "Comfortable shoes", "Compact umbrella"],
                    it: ["Abbigliamento leggero e comodo", "Scarpe comode", "Ombrello compatto"]
                }
            },

            sidebar: {
                duracion: { es: "5 días", en: "5 days", it: "5 giorni" },
                epoca: { es: "Mar-Oct", en: "Mar-Oct", it: "Mar-Ott" },
                presupuesto: "70–100 €/día",
                intensidad: { es: "Alta", en: "High", it: "Alta" }
            }
        }
    },

    // --- GUÍA 7: PARÍS ---
    {
        id: 7,
        titulo: {
            es: "París en 3 días: arte y cultura",
            en: "Paris in 3 days: art and culture",
            it: "Parigi in 3 giorni: arte e cultura"
        },
        categoria: { es: "Viajes culturales", en: "Cultural trips", it: "Viaggi culturali" },
        destino: "Francia",
        tipo: { es: "Ruta cultural", en: "Cultural route", it: "Percorso culturale" },
        imagen: "https://images.unsplash.com/photo-1502602898657-3e91760cbb34?w=800",
        lectura: "10 min",
        valoracion: "4.8",
        desc: {
            es: "Descubre los museos, barrios y monumentos más importantes de París en 3 días.",
            en: "Discover Paris's top museums, neighborhoods, and monuments in 3 days.",
            it: "Scopri musei, quartieri e monumenti più importanti di Parigi in 3 giorni."
        },
        tags: ["Francia", "Arte", "Cultura"],

        detalle: {
            subtitulo: { es: "La ciudad del arte y la historia.", en: "The city of art and history.", it: "La città dell'arte e della storia." },
            autor: "France Travel",
            intro: { es: "París es un museo al aire libre con historia, arte y gastronomía.", en: "Paris is an open-air museum with history, art and gastronomy.", it: "Parigi è un museo a cielo aperto con storia, arte e gastronomia." },

            itinerario: [
                { dia: 1, titulo: { es: "Museo del Louvre", en: "Louvre Museum", it: "Museo del Louvre" }, desc: { es: "Obras maestras y arquitectura.", en: "Masterpieces and architecture.", it: "Capolavori e architettura." } },
                { dia: 2, titulo: { es: "Barrio Latino", en: "Latin Quarter", it: "Quartiere Latino" }, desc: { es: "Calles, cafés y librerías.", en: "Streets, cafes and bookstores.", it: "Strade, caffè e librerie." } },
                { dia: 3, titulo: { es: "Torre Eiffel y Montmartre", en: "Eiffel Tower and Montmartre", it: "Torre Eiffel e Montmartre" }, desc: { es: "Vistas panorámicas y arte callejero.", en: "Panoramic views and street art.", it: "Vedute panoramiche e arte di strada." } }
            ],

            consejos: {
                es: ["Compra entradas online", "Camina para descubrir barrios", "Prueba la gastronomía local"],
                en: ["Buy tickets online", "Walk to discover neighborhoods", "Try local gastronomy"],
                it: ["Acquista biglietti online", "Cammina per scoprire i quartieri", "Prova la gastronomia locale"]
            },

            maleta: {
                clima: { es: "Templado", en: "Mild", it: "Mite" },
                items: {
                    es: ["Ropa cómoda", "Zapatos para caminar", "Cámara"],
                    en: ["Comfortable clothes", "Walking shoes", "Camera"],
                    it: ["Abbigliamento comodo", "Scarpe da passeggio", "Fotocamera"]
                }
            },

            sidebar: {
                duracion: { es: "3 días", en: "3 days", it: "3 giorni" },
                epoca: { es: "Abr-Jun, Sep-Oct", en: "Apr-Jun, Sep-Oct", it: "Apr-Giu, Set-Ott" },
                presupuesto: "60–90 €/día",
                intensidad: { es: "Media", en: "Medium", it: "Media" }
            }
        }
    },

    // --- GUÍA 8: BARCELONA ---
    {
        id: 8,
        titulo: {
            es: "Barcelona en 3 días: arte y cultura",
            en: "Barcelona in 3 days: art and culture",
            it: "Barcellona in 3 giorni: arte e cultura"
        },
        categoria: { es: "Viajes culturales", en: "Cultural trips", it: "Viaggi culturali" },
        destino: "España",
        tipo: { es: "Ruta cultural", en: "Cultural route", it: "Percorso culturale" },
        imagen: "https://images.unsplash.com/photo-1508174698850-1a676f3a6a27?w=800",
        lectura: "10 min",
        valoracion: "4.7",
        desc: {
            es: "Explora la arquitectura, barrios y museos más importantes de Barcelona en 3 días.",
            en: "Explore Barcelona's main architecture, neighborhoods, and museums in 3 days.",
            it: "Esplora architettura, quartieri e musei principali di Barcellona in 3 giorni."
        },
        tags: ["España", "Arte", "Cultura"],

        detalle: {
            subtitulo: { es: "Barcelona, Gaudí y más.", en: "Barcelona, Gaudí and more.", it: "Barcellona, Gaudí e altro." },
            autor: "Spain Travel",
            intro: { es: "Una ciudad llena de arte, historia y vida urbana.", en: "A city full of art, history, and urban life.", it: "Una città piena di arte, storia e vita urbana." },

            itinerario: [
                { dia: 1, titulo: { es: "Sagrada Familia", en: "Sagrada Familia", it: "Sagrada Familia" }, desc: { es: "Icono de Gaudí.", en: "Gaudí's icon.", it: "Icona di Gaudí." } },
                { dia: 2, titulo: { es: "Barrio Gótico", en: "Gothic Quarter", it: "Quartiere Gotico" }, desc: { es: "Calles históricas y plazas.", en: "Historic streets and squares.", it: "Strade e piazze storiche." } },
                { dia: 3, titulo: { es: "Parque Güell", en: "Park Güell", it: "Parco Güell" }, desc: { es: "Arte y vistas panorámicas.", en: "Art and panoramic views.", it: "Arte e vedute panoramiche." } }
            ],

            consejos: {
                es: ["Compra entradas anticipadas", "Usa transporte público", "Camina para descubrir detalles"],
                en: ["Buy tickets in advance", "Use public transport", "Walk to discover details"],
                it: ["Acquista biglietti in anticipo", "Usa trasporto pubblico", "Cammina per scoprire dettagli"]
            },

            maleta: {
                clima: { es: "Templado", en: "Mild", it: "Mite" },
                items: {
                    es: ["Ropa cómoda", "Zapatos para caminar", "Sombrero o gorra"],
                    en: ["Comfortable clothes", "Walking shoes", "Hat or cap"],
                    it: ["Abbigliamento comodo", "Scarpe da passeggio", "Cappello"]
                }
            },

            sidebar: {
                duracion: { es: "3 días", en: "3 days", it: "3 giorni" },
                epoca: { es: "Abr-Jun, Sep-Oct", en: "Apr-Jun, Sep-Oct", it: "Apr-Giu, Set-Ott" },
                presupuesto: "50–80 €/día",
                intensidad: { es: "Media", en: "Medium", it: "Media" }
            }
        }
    },

    // --- GUÍA 9: AMSTERDAM ---
    {
        id: 9,
        titulo: {
            es: "Ámsterdam en 3 días: cultura y canales",
            en: "Amsterdam in 3 days: culture and canals",
            it: "Amsterdam in 3 giorni: cultura e canali"
        },
        categoria: { es: "Viajes culturales", en: "Cultural trips", it: "Viaggi culturali" },
        destino: "Países Bajos",
        tipo: { es: "Ruta cultural", en: "Cultural route", it: "Percorso culturale" },
        imagen: "https://images.unsplash.com/photo-1528299100171-1efb0f0b94f2?w=800",
        lectura: "10 min",
        valoracion: "4.7",
        desc: {
            es: "Descubre museos, canales y barrios históricos de Ámsterdam en un itinerario de 3 días.",
            en: "Discover museums, canals, and historic neighborhoods in Amsterdam in a 3-day itinerary.",
            it: "Scopri musei, canali e quartieri storici di Amsterdam in un itinerario di 3 giorni."
        },
        tags: ["Países Bajos", "Cultura", "Ciudad"],

        detalle: {
            subtitulo: { es: "Ámsterdam, historia y canales.", en: "Amsterdam, history and canals.", it: "Amsterdam, storia e canali." },
            autor: "Netherlands Travel",
            intro: { es: "Una ciudad compacta perfecta para recorrer a pie o en bicicleta.", en: "A compact city perfect to explore by foot or bike.", it: "Una città compatta perfetta da esplorare a piedi o in bici." },

            itinerario: [
                { dia: 1, titulo: { es: "Museo Van Gogh", en: "Van Gogh Museum", it: "Museo Van Gogh" }, desc: { es: "Arte y exposiciones.", en: "Art and exhibitions.", it: "Arte e mostre." } },
                { dia: 2, titulo: { es: "Canales y Jordaan", en: "Canals and Jordaan", it: "Canali e Jordaan" }, desc: { es: "Paseos y arquitectura típica.", en: "Walks and typical architecture.", it: "Passeggiate e architettura tipica." } },
                { dia: 3, titulo: { es: "Museo Anne Frank", en: "Anne Frank Museum", it: "Museo di Anne Frank" }, desc: { es: "Historia y memoria.", en: "History and memory.", it: "Storia e memoria." } }
            ],

            consejos: {
                es: ["Alquila bicicleta", "Compra entradas online", "Camina por los barrios"],
                en: ["Rent a bike", "Buy tickets online", "Walk through neighborhoods"],
                it: ["Noleggia bicicletta", "Acquista biglietti online", "Cammina nei quartieri"]
            },

            maleta: {
                clima: { es: "Templado y húmedo", en: "Mild and humid", it: "Mite e umido" },
                items: {
                    es: ["Ropa cómoda", "Impermeable ligero", "Zapatos cerrados"],
                    en: ["Comfortable clothes", "Light raincoat", "Closed shoes"],
                    it: ["Abbigliamento comodo", "Impermeabile leggero", "Scarpe chiuse"]
                }
            },

            sidebar: {
                duracion: { es: "3 días", en: "3 days", it: "3 giorni" },
                epoca: { es: "Abr-Sep", en: "Apr-Sep", it: "Apr-Set" },
                presupuesto: "60–90 €/día",
                intensidad: { es: "Media", en: "Medium", it: "Media" }
            }
        }
    },

    // --- GUÍA 10: BERLÍN ---
    {
        id: 10,
        titulo: {
            es: "Berlín en 3 días: historia y cultura",
            en: "Berlin in 3 days: history and culture",
            it: "Berlino in 3 giorni: storia e cultura"
        },
        categoria: { es: "Viajes culturales", en: "Cultural trips", it: "Viaggi culturali" },
        destino: "Alemania",
        tipo: { es: "Ruta cultural", en: "Cultural route", it: "Percorso culturale" },
        imagen: "https://images.unsplash.com/photo-1509099836639-18ba4c1ee1d6?w=800",
        lectura: "11 min",
        valoracion: "4.7",
        desc: {
            es: "Recorre la historia, museos y barrios alternativos de Berlín en 3 días.",
            en: "Explore Berlin's history, museums, and alternative neighborhoods in 3 days.",
            it: "Esplora storia, musei e quartieri alternativi di Berlino in 3 giorni."
        },
        tags: ["Alemania", "Historia", "Cultura"],

        detalle: {
            subtitulo: { es: "Berlín, historia reciente y arte urbano.", en: "Berlin, recent history and urban art.", it: "Berlino, storia recente e arte urbana." },
            autor: "Germany Travel",
            intro: { es: "Una ciudad marcada por la historia del siglo XX y con cultura vibrante.", en: "A city marked by 20th-century history and vibrant culture.", it: "Una città segnata dalla storia del XX secolo e cultura vibrante." },

            itinerario: [
                { dia: 1, titulo: { es: "Puerta de Brandeburgo y Reichstag", en: "Brandenburg Gate and Reichstag", it: "Porta di Brandeburgo e Reichstag" }, desc: { es: "Historia y arquitectura.", en: "History and architecture.", it: "Storia e architettura." } },
                { dia: 2, titulo: { es: "Museo de Pérgamo", en: "Pergamon Museum", it: "Museo di Pergamo" }, desc: { es: "Antigüedades y arte.", en: "Antiquities and art.", it: "Antichità e arte." } },
                { dia: 3, titulo: { es: "East Side Gallery", en: "East Side Gallery", it: "East Side Gallery" }, desc: { es: "Grafitis y memoria histórica.", en: "Graffiti and historical memory.", it: "Graffiti e memoria storica." } }
            ],

            consejos: {
                es: ["Compra tickets online", "Usa transporte público", "Explora barrios alternativos"],
                en: ["Buy tickets online", "Use public transport", "Explore alternative neighborhoods"],
                it: ["Acquista biglietti online", "Usa trasporto pubblico", "Esplora quartieri alternativi"]
            },

            maleta: {
                clima: { es: "Templado", en: "Mild", it: "Mite" },
                items: {
                    es: ["Ropa cómoda", "Zapatos para caminar", "Capa ligera"],
                    en: ["Comfortable clothes", "Walking shoes", "Light layer"],
                    it: ["Abbigliamento comodo", "Scarpe da passeggio", "Strato leggero"]
                }
            },

            sidebar: {
                duracion: { es: "3 días", en: "3 days", it: "3 giorni" },
                epoca: { es: "Abr-Oct", en: "Apr-Oct", it: "Apr-Ott" },
                presupuesto: "50–80 €/día",
                intensidad: { es: "Media", en: "Medium", it: "Media" }
            }
        }
    },


    // ==================================================
    // GUÍA 11: MARRUECOS — MARRAKECH
    // ==================================================
    {
        id: 11,
        titulo: {
            es: "Marrakech en 3 días: caos, cultura y zocos",
            en: "Marrakech in 3 days: chaos, culture and souks",
            it: "Marrakech in 3 giorni: caos, cultura e souk"
        },
        categoria: {
            es: "Viajes culturales",
            en: "Cultural trips",
            it: "Viaggi culturali"
        },
        destino: "Marruecos",
        tipo: {
            es: "Ruta urbana",
            en: "Urban route",
            it: "Percorso urbano"
        },
        imagen: "https://images.unsplash.com/photo-1548013146-72479768bada?w=800",
        lectura: "10 min",
        valoracion: "4.7",
        desc: {
            es: "Marrakech es una ciudad intensa, ruidosa y fascinante a partes iguales. En esta guía te proponemos una ruta equilibrada de 3 días para descubrir sus monumentos más importantes, perderte por los zocos y entender su cultura sin sentirte abrumado.",
            en: "Marrakech is an intense, noisy and fascinating city. This 3-day route helps you discover its main monuments, explore the souks and understand the culture without feeling overwhelmed.",
            it: "Marrakech è una città intensa, rumorosa e affascinante. Questa guida propone un itinerario di 3 giorni per scoprire i monumenti principali e i souk senza sentirsi sopraffatti."
        },
        tags: ["África", "Cultura", "Zocos"],

        detalle: {
            subtitulo: {
                es: "Una introducción perfecta a Marruecos.",
                en: "A perfect introduction to Morocco.",
                it: "Un'introduzione perfetta al Marocco."
            },
            autor: "Mochileros Staff",
            intro: {
                es: "Marrakech es ideal para un primer contacto con el norte de África.",
                en: "Marrakech is ideal for a first contact with North Africa.",
                it: "Marrakech è ideale per un primo contatto con il Nord Africa."
            },

            itinerario: [
                {
                    dia: 1,
                    titulo: { es: "Medina", en: "Medina", it: "Medina" },
                    desc: {
                        es: "Plaza Jemaa el-Fna y zocos.",
                        en: "Jemaa el-Fna square and souks.",
                        it: "Piazza Jemaa el-Fna e souk."
                    }
                },
                {
                    dia: 2,
                    titulo: { es: "Palacios", en: "Palaces", it: "Palazzi" },
                    desc: {
                        es: "Bahía y El Badi.",
                        en: "Bahia and El Badi.",
                        it: "Bahia ed El Badi."
                    }
                },
                {
                    dia: 3,
                    titulo: { es: "Jardines", en: "Gardens", it: "Giardini" },
                    desc: {
                        es: "Majorelle y barrio moderno.",
                        en: "Majorelle and modern district.",
                        it: "Majorelle e quartiere moderno."
                    }
                }
            ],

            consejos: {
                es: ["Negocia siempre precios.", "Respeta el código de vestimenta.", "Evita horas punta en la plaza."],
                en: ["Always negotiate prices.", "Respect dress code.", "Avoid peak hours at the square."],
                it: ["Contratta sempre i prezzi.", "Rispetta il codice di abbigliamento.", "Evita le ore di punta."]
            },

            maleta: {
                clima: {
                    es: "Seco y caluroso",
                    en: "Dry and hot",
                    it: "Secco e caldo"
                },
                items: {
                    es: ["Ropa holgada", "Pañuelo", "Protector solar"],
                    en: ["Loose clothing", "Scarf", "Sunscreen"],
                    it: ["Abiti larghi", "Sciarpa", "Crema solare"]
                }
            },

            sidebar: {
                duracion: { es: "3 días", en: "3 days", it: "3 giorni" },
                epoca: { es: "Mar-May / Sep-Nov", en: "Mar-May / Sep-Nov", it: "Mar-Mag / Set-Nov" },
                presupuesto: "40–60 € / día",
                intensidad: { es: "Media", en: "Medium", it: "Media" }
            }
        }
    },

    // ==================================================
    // GUÍA 12: PERÚ — MACHU PICCHU
    // ==================================================
    {
        id: 12,
        titulo: {
            es: "Machu Picchu sin tour: cómo organizarlo",
            en: "Machu Picchu without a tour: how to organize it",
            it: "Machu Picchu senza tour: come organizzarlo"
        },
        categoria: {
            es: "Aventura",
            en: "Adventure",
            it: "Avventura"
        },
        destino: "Perú",
        tipo: {
            es: "Guía práctica",
            en: "Practical guide",
            it: "Guida pratica"
        },
        imagen: "https://images.unsplash.com/photo-1505678261036-a3fcc5e884ee?w=800",
        lectura: "14 min",
        valoracion: "4.9",
        desc: {
            es: "Llegar a Machu Picchu sin contratar un tour cerrado es totalmente posible y puede ahorrarte bastante dinero. Te explicamos cómo llegar desde Cusco, qué entradas comprar y qué errores evitar.",
            en: "Reaching Machu Picchu without a tour is possible and can save you money. Learn how to get there from Cusco, which tickets to buy and common mistakes to avoid.",
            it: "Raggiungere Machu Picchu senza tour è possibile e più economico. Scopri come arrivare da Cusco e quali errori evitare."
        },
        tags: ["Montaña", "Historia", "Trekking"],

        detalle: {
            subtitulo: {
                es: "La maravilla inca por libre.",
                en: "The Inca wonder independently.",
                it: "La meraviglia inca in autonomia."
            },
            autor: "Peru Travel",
            intro: {
                es: "Con organización, Machu Picchu puede visitarse sin agencias.",
                en: "With planning, Machu Picchu can be visited without agencies.",
                it: "Con una buona organizzazione, Machu Picchu si può visitare senza agenzie."
            },

            itinerario: [
                {
                    dia: 1,
                    titulo: { es: "Cusco", en: "Cusco", it: "Cusco" },
                    desc: {
                        es: "Aclimatación y preparativos.",
                        en: "Acclimatization and preparation.",
                        it: "Acclimatazione e preparativi."
                    }
                },
                {
                    dia: 2,
                    titulo: { es: "Aguas Calientes", en: "Aguas Calientes", it: "Aguas Calientes" },
                    desc: {
                        es: "Viaje en tren o ruta alternativa.",
                        en: "Train journey or alternative route.",
                        it: "Viaggio in treno o percorso alternativo."
                    }
                },
                {
                    dia: 3,
                    titulo: { es: "Machu Picchu", en: "Machu Picchu", it: "Machu Picchu" },
                    desc: {
                        es: "Visita y regreso.",
                        en: "Visit and return.",
                        it: "Visita e rientro."
                    }
                }
            ],

            consejos: {
                es: ["Compra entradas con antelación.", "Aclimata bien a la altura.", "Lleva efectivo."],
                en: ["Buy tickets in advance.", "Acclimatize properly.", "Carry cash."],
                it: ["Compra i biglietti in anticipo.", "Acclimatati bene.", "Porta contanti."]
            },

            maleta: {
                clima: {
                    es: "Variable y húmedo",
                    en: "Variable and humid",
                    it: "Variabile e umido"
                },
                items: {
                    es: ["Impermeable", "Calzado trekking", "Protector solar"],
                    en: ["Rain jacket", "Trekking shoes", "Sunscreen"],
                    it: ["Impermeabile", "Scarpe da trekking", "Crema solare"]
                }
            },

            sidebar: {
                duracion: { es: "3 días", en: "3 days", it: "3 giorni" },
                epoca: { es: "Abr-Oct", en: "Apr-Oct", it: "Apr-Ott" },
                presupuesto: "70–100 € / día",
                intensidad: { es: "Alta", en: "High", it: "Alta" }
            }
        }
    },


    // ==================================================
    // GUÍA 21: TAILANDIA
    // ==================================================
    {
        id: 21,
        titulo: {
            es: "Tailandia mochilera: ruta clásica para primer viaje",
            en: "Backpacking Thailand: classic route for first trip",
            it: "Thailandia zaino in spalla: percorso classico per il primo viaggio"
        },
        categoria: {
            es: "Presupuesto bajo",
            en: "Low budget",
            it: "Basso budget"
        },
        destino: "Tailandia",
        tipo: {
            es: "Ruta",
            en: "Route",
            it: "Itinerario"
        },
        imagen: "https://images.unsplash.com/photo-1504214208698-ea1916a2195a?w=800",
        lectura: "17 min",
        valoracion: "4.9",
        desc: {
            es: "Tailandia es uno de los destinos mochileros más completos del mundo. Esta ruta está pensada para quienes viajan por primera vez al sudeste asiático y buscan una combinación equilibrada de cultura, playas, naturaleza y precios bajos.",
            en: "Thailand is one of the most complete backpacking destinations in the world. This route is for first-time travelers to Southeast Asia looking for a balanced mix of culture, beaches, nature, and low costs.",
            it: "La Thailandia è una delle destinazioni zaino in spalla più complete del mondo. Questo itinerario è pensato per chi viaggia per la prima volta nel sud-est asiatico."
        },
        tags: ["Asia", "Backpacker", "Low cost"],

        detalle: {
            subtitulo: {
                es: "El mejor punto de entrada al sudeste asiático.",
                en: "The best entry point to Southeast Asia.",
                it: "Il miglior punto d'ingresso nel sud-est asiatico."
            },
            autor: "Backpack Asia",
            intro: {
                es: "Tailandia es fácil, segura y muy económica para empezar.",
                en: "Thailand is easy, safe, and very affordable to start.",
                it: "La Thailandia è facile, sicura e molto economica per iniziare."
            },

            itinerario: [
                { dia: "1-4", titulo: { es: "Bangkok", en: "Bangkok", it: "Bangkok" }, desc: { es: "Templos, mercados y vida urbana.", en: "Temples, markets, and urban life.", it: "Templi, mercati e vita urbana." } },
                { dia: "5-7", titulo: { es: "Chiang Mai", en: "Chiang Mai", it: "Chiang Mai" }, desc: { es: "Naturaleza y templos del norte.", en: "Nature and northern temples.", it: "Natura e templi del nord." } },
                { dia: "8-14", titulo: { es: "Islas", en: "Islands", it: "Isole" }, desc: { es: "Playas y snorkel.", en: "Beaches and snorkeling.", it: "Spiagge e snorkeling." } }
            ],

            consejos: {
                es: ["Come en puestos callejeros.", "Muévete en tren nocturno.", "Negocia en mercados."],
                en: ["Eat at street stalls.", "Travel by night train.", "Negotiate at markets."],
                it: ["Mangia ai chioschi.", "Spostati con treno notturno.", "Contratta nei mercati."]
            },

            maleta: {
                clima: { es: "Tropical", en: "Tropical", it: "Tropicale" },
                items: { es: ["Ropa ligera", "Sandalias", "Repelente"], en: ["Light clothes", "Sandals", "Repellent"], it: ["Abiti leggeri", "Sandali", "Repellente"] }
            },

            sidebar: {
                duracion: { es: "14 días", en: "14 days", it: "14 giorni" },
                epoca: { es: "Nov-Feb", en: "Nov-Feb", it: "Nov-Feb" },
                presupuesto: "25–40 €/día",
                intensidad: { es: "Media", en: "Medium", it: "Media" }
            }
        }
    },

    // ==================================================
    // GUÍA 22: COLOMBIA
    // ==================================================
    {
        id: 22,
        titulo: {
            es: "Colombia segura: qué saber antes de viajar",
            en: "Safe Colombia: what to know before traveling",
            it: "Colombia sicura: cosa sapere prima di viaggiare"
        },
        categoria: {
            es: "Clima y mejor época",
            en: "Climate & best time",
            it: "Clima e periodo migliore"
        },
        destino: "Colombia",
        tipo: {
            es: "Consejos",
            en: "Tips",
            it: "Consigli"
        },
        imagen: "https://images.unsplash.com/photo-1526413236354-94cdf54aff45?w=800",
        lectura: "11 min",
        valoracion: "4.7",
        desc: {
            es: "Colombia ha cambiado mucho en los últimos años y hoy es un destino vibrante y acogedor. En esta guía te contamos qué zonas son más seguras, cuándo viajar según el clima y cómo moverte con tranquilidad.",
            en: "Colombia has changed a lot in recent years and is now a vibrant and welcoming destination. This guide tells you which areas are safest, the best travel periods, and how to move around safely.",
            it: "La Colombia è cambiata molto negli ultimi anni ed è oggi una destinazione vibrante e accogliente. Questa guida indica le zone più sicure e come muoversi."
        },
        tags: ["Sudamérica", "Consejos"],

        detalle: {
            subtitulo: {
                es: "Rompiendo mitos sobre Colombia.",
                en: "Breaking myths about Colombia.",
                it: "Sfondare i miti sulla Colombia."
            },
            autor: "Colombia Travel",
            intro: {
                es: "Con información adecuada, Colombia es un destino increíble.",
                en: "With proper information, Colombia is an amazing destination.",
                it: "Con le giuste informazioni, la Colombia è una destinazione incredibile."
            },

            itinerario: [
                { dia: "1-3", titulo: { es: "Bogotá", en: "Bogotá", it: "Bogotá" }, desc: { es: "Cultura y museos.", en: "Culture and museums.", it: "Cultura e musei." } },
                { dia: "4-6", titulo: { es: "Medellín", en: "Medellín", it: "Medellín" }, desc: { es: "Innovación y barrios.", en: "Innovation and neighborhoods.", it: "Innovazione e quartieri." } },
                { dia: "7-10", titulo: { es: "Caribe", en: "Caribbean", it: "Caraibi" }, desc: { es: "Playas y descanso.", en: "Beaches and rest.", it: "Spiagge e relax." } }
            ],

            consejos: {
                es: ["Evita zonas poco transitadas de noche.", "Usa Uber o taxis oficiales.", "Infórmate del clima local."],
                en: ["Avoid isolated areas at night.", "Use Uber or official taxis.", "Check local weather."],
                it: ["Evita zone poco trafficate di notte.", "Usa Uber o taxi ufficiali.", "Controlla il clima locale."]
            },

            maleta: {
                clima: { es: "Variable", en: "Variable", it: "Variabile" },
                items: { es: ["Capas", "Impermeable ligero"], en: ["Layers", "Light raincoat"], it: ["Strati", "Impermeabile leggero"] }
            },

            sidebar: {
                duracion: { es: "10 días", en: "10 days", it: "10 giorni" },
                epoca: { es: "Dic-Mar / Jul-Ago", en: "Dec-Mar / Jul-Aug", it: "Dic-Mar / Lug-Ago" },
                presupuesto: "40–60 €/día",
                intensidad: { es: "Media", en: "Medium", it: "Media" }
            }
        }
    },

    // ==================================================
    // GUÍAS 23–30
    // ==================================================

// ======================================================
// GUÍAS "QUÉ LLEVAR" — IDS 31 A 35 (MULTIDIOMA)
// ======================================================

    // ==================================================
    // GUÍA 31: MOCHILA GENERAL
    // ==================================================
    {
        id: 31,
        titulo: {
            es: "Qué llevar en la mochila para un viaje largo",
            en: "What to pack in your backpack for a long trip",
            it: "Cosa mettere nello zaino per un viaggio lungo"
        },
        categoria: {
            es: "Qué llevar",
            en: "Packing",
            it: "Cosa portare"
        },
        destino: "General",
        tipo: {
            es: "Checklist",
            en: "Checklist",
            it: "Checklist"
        },
        imagen: "https://images.unsplash.com/photo-1500530855697-b586d89ba3ee?w=800",
        lectura: "10 min",
        valoracion: "4.9",
        desc: {
            es: "Preparar la mochila para un viaje largo puede ser complicado si no sabes por dónde empezar. En esta guía te explicamos qué llevar realmente, qué es prescindible y cómo organizar tu equipaje para viajar ligero y cómodo durante semanas o meses.",
            en: "Packing a backpack for a long trip can be tricky if you don't know where to start. This guide explains what to bring, what's optional, and how to organize your gear for a light and comfortable journey.",
            it: "Preparare lo zaino per un viaggio lungo può essere complicato se non sai da dove iniziare. Questa guida spiega cosa portare, cosa è opzionale e come organizzare il bagaglio per viaggiare leggero."
        },
        tags: ["Mochila", "Equipaje", "Consejos"],

        detalle: {
            subtitulo: {
                es: "Lo imprescindible para viajar ligero sin echar nada en falta.",
                en: "The essentials for traveling light without missing anything.",
                it: "L'essenziale per viaggiare leggeri senza rinunciare a nulla."
            },
            autor: "Mochileros Staff",
            intro: {
                es: "Una mochila bien preparada marca la diferencia entre disfrutar el viaje o cargar peso innecesario.",
                en: "A well-prepared backpack makes the difference between enjoying the trip or carrying unnecessary weight.",
                it: "Uno zaino ben preparato fa la differenza tra godersi il viaggio o portare peso inutile."
            },

            itinerario: [],

            consejos: {
                es: ["No llenes la mochila el primer día.", "Prioriza ropa versátil.", "Deja espacio para souvenirs."],
                en: ["Don't fill the backpack on the first day.", "Prioritize versatile clothing.", "Leave space for souvenirs."],
                it: ["Non riempire lo zaino il primo giorno.", "Dai priorità a vestiti versatili.", "Lascia spazio per i souvenir."]
            },

            maleta: {
                clima: { es: "Variable", en: "Variable", it: "Variabile" },
                items: {
                    es: ["Mochila 40–50L", "Ropa básica y capas", "Neceser compacto", "Chanclas", "Bolsa para ropa sucia"],
                    en: ["40–50L backpack", "Basic clothes and layers", "Compact toiletry bag", "Flip-flops", "Bag for dirty clothes"],
                    it: ["Zaino 40–50L", "Abbigliamento base e strati", "Beauty case compatto", "Infradito", "Borsa per vestiti sporchi"]
                }
            },

            sidebar: {
                duracion: { es: "Aplicable a cualquier viaje", en: "Applicable to any trip", it: "Applicabile a qualsiasi viaggio" },
                epoca: { es: "Todo el año", en: "All year", it: "Tutto l'anno" },
                presupuesto: "Según equipamiento",
                intensidad: { es: "Baja", en: "Low", it: "Bassa" }
            }
        }
    },

    // ==================================================
    // GUÍA 32: CLIMA FRÍO
    // ==================================================
    {
        id: 32,
        titulo: {
            es: "Qué llevar para viajar a destinos fríos",
            en: "What to pack for cold destinations",
            it: "Cosa portare per destinazioni fredde"
        },
        categoria: {
            es: "Qué llevar",
            en: "Packing",
            it: "Cosa portare"
        },
        destino: "Clima frío",
        tipo: {
            es: "Guía práctica",
            en: "Practical guide",
            it: "Guida pratica"
        },
        imagen: "https://images.unsplash.com/photo-1519681393784-d120267933ba?w=800",
        lectura: "8 min",
        valoracion: "4.8",
        desc: {
            es: "Viajar a destinos fríos no significa llevar mil prendas pesadas. En esta guía te enseñamos cómo vestirte por capas, qué ropa es realmente necesaria y qué errores evitar para mantenerte caliente sin cargar peso.",
            en: "Traveling to cold destinations doesn't mean packing a ton of heavy clothes. This guide shows how to layer properly, what's really necessary, and mistakes to avoid to stay warm without extra weight.",
            it: "Viaggiare in destinazioni fredde non significa portare mille vestiti pesanti. Questa guida insegna come vestirsi a strati, cosa è davvero necessario e quali errori evitare per rimanere al caldo."
        },
        tags: ["Frío", "Ropa térmica"],

        detalle: {
            subtitulo: {
                es: "Calor sin exceso de peso.",
                en: "Warmth without excess weight.",
                it: "Calore senza peso eccessivo."
            },
            autor: "Outdoor Travel",
            intro: {
                es: "El sistema de capas es la clave para el frío.",
                en: "Layering is key for cold weather.",
                it: "Il sistema a strati è la chiave per il freddo."
            },

            itinerario: [],

            consejos: {
                es: ["Invierte en una buena capa térmica.", "Evita algodón.", "Protege extremidades."],
                en: ["Invest in good thermal layers.", "Avoid cotton.", "Protect extremities."],
                it: ["Investi in buoni strati termici.", "Evita il cotone.", "Proteggi estremità."]
            },

            maleta: {
                clima: { es: "Frío", en: "Cold", it: "Freddo" },
                items: {
                    es: ["Camisetas térmicas", "Plumón ligero", "Gorro y guantes", "Calcetines térmicos", "Botas impermeables"],
                    en: ["Thermal shirts", "Light down jacket", "Hat and gloves", "Thermal socks", "Waterproof boots"],
                    it: ["Magliette termiche", "Piumino leggero", "Cappello e guanti", "Calze termiche", "Scarpe impermeabili"]
                }
            },

            sidebar: {
                duracion: { es: "Viajes de invierno", en: "Winter trips", it: "Viaggi invernali" },
                epoca: { es: "Invierno", en: "Winter", it: "Inverno" },
                presupuesto: "Medio",
                intensidad: { es: "Baja", en: "Low", it: "Bassa" }
            }
        }
    },

    // ==================================================
    // GUÍA 33: CLIMA TROPICAL
    // ==================================================
    {
        id: 33,
        titulo: {
            es: "Qué llevar a destinos tropicales y húmedos",
            en: "What to pack for tropical and humid destinations",
            it: "Cosa portare per destinazioni tropicali e umide"
        },
        categoria: {
            es: "Qué llevar",
            en: "Packing",
            it: "Cosa portare"
        },
        destino: "Clima tropical",
        tipo: {
            es: "Checklist",
            en: "Checklist",
            it: "Checklist"
        },
        imagen: "https://images.unsplash.com/photo-1507525428034-b723cf961d3e?w=800",
        lectura: "9 min",
        valoracion: "4.9",
        desc: {
            es: "El calor y la humedad pueden arruinar un viaje si no llevas el equipamiento adecuado. Esta guía te ayuda a elegir ropa transpirable, protegerte del sol y evitar problemas comunes en destinos tropicales.",
            en: "Heat and humidity can ruin a trip if you don't have the right gear. This guide helps you choose breathable clothing, sun protection, and avoid common issues in tropical destinations.",
            it: "Il caldo e l'umidità possono rovinare un viaggio se non si ha l'equipaggiamento giusto. Questa guida aiuta a scegliere vestiti traspiranti, protezione solare e evitare problemi comuni."
        },
        tags: ["Calor", "Tropical"],

        detalle: {
            subtitulo: {
                es: "Ligero, fresco y funcional.",
                en: "Light, fresh, and functional.",
                it: "Leggero, fresco e funzionale."
            },
            autor: "Asia Backpack",
            intro: {
                es: "Menos es más cuando viajas al trópico.",
                en: "Less is more when traveling to the tropics.",
                it: "Meno è meglio quando si viaggia nei tropici."
            },

            itinerario: [],

            consejos: {
                es: ["Evita tejidos sintéticos.", "Usa repelente siempre.", "Hidrátate constantemente."],
                en: ["Avoid synthetic fabrics.", "Always use insect repellent.", "Stay hydrated."],
                it: ["Evita tessuti sintetici.", "Usa sempre repellente.", "Idratati costantemente."]
            },

            maleta: {
                clima: { es: "Caluroso y húmedo", en: "Hot & humid", it: "Caldo e umido" },
                items: {
                    es: ["Ropa de algodón o lino", "Repelente de insectos", "Protector solar", "Impermeable ligero", "Sandalias cómodas"],
                    en: ["Cotton or linen clothes", "Insect repellent", "Sunscreen", "Light raincoat", "Comfortable sandals"],
                    it: ["Abiti in cotone o lino", "Repellente per insetti", "Protezione solare", "Impermeabile leggero", "Sandali comodi"]
                }
            },

            sidebar: {
                duracion: { es: "Viajes tropicales", en: "Tropical trips", it: "Viaggi tropicali" },
                epoca: { es: "Todo el año", en: "All year", it: "Tutto l'anno" },
                presupuesto: "Bajo",
                intensidad: { es: "Baja", en: "Low", it: "Bassa" }
            }
        }
    },

    // ==================================================
    // GUÍA 34: TREKKING
    // ==================================================
    {
        id: 34,
        titulo: {
            es: "Qué llevar para un trekking de varios días",
            en: "What to pack for a multi-day trekking",
            it: "Cosa portare per un trekking di più giorni"
        },
        categoria: {
            es: "Qué llevar",
            en: "Packing",
            it: "Cosa portare"
        },
        destino: "Montaña",
        tipo: {
            es: "Guía técnica",
            en: "Technical guide",
            it: "Guida tecnica"
        },
        imagen: "https://images.unsplash.com/photo-1500534314209-a25ddb2bd429?w=800",
        lectura: "12 min",
        valoracion: "4.8",
        desc: {
            es: "Un trekking de varios días requiere una preparación especial. En esta guía detallamos el equipamiento básico, la ropa adecuada y los errores más comunes que cometen los viajeros al preparar su mochila.",
            en: "A multi-day trek requires special preparation. This guide details the basic gear, proper clothing, and common mistakes trekkers make when packing.",
            it: "Un trekking di più giorni richiede una preparazione speciale. Questa guida dettaglia l'equipaggiamento base, l'abbigliamento adeguato e gli errori comuni."
        },
        tags: ["Trekking", "Montaña"],

        detalle: {
            subtitulo: {
                es: "Preparación es seguridad.",
                en: "Preparation is safety.",
                it: "Preparazione è sicurezza."
            },
            autor: "Mountain Guides",
            intro: {
                es: "Llevar lo justo y necesario es vital en montaña.",
                en: "Carrying only what is necessary is vital in the mountains.",
                it: "Portare solo l'essenziale è vitale in montagna."
            },

            itinerario: [],

            consejos: {
                es: ["No estrenes botas.", "Reduce peso innecesario.", "Lleva botiquín básico."],
                en: ["Don't wear new boots.", "Reduce unnecessary weight.", "Bring a basic first aid kit."],
                it: ["Non indossare scarpe nuove.", "Riduci peso inutile.", "Porta un kit di pronto soccorso base."]
            },

            maleta: {
                clima: { es: "Variable de montaña", en: "Mountain variable", it: "Variabile montagna" },
                items: {
                    es: ["Botas de trekking", "Mochila con soporte lumbar", "Ropa técnica", "Saco de dormir", "Linterna frontal"],
                    en: ["Trekking boots", "Backpack with lumbar support", "Technical clothing", "Sleeping bag", "Headlamp"],
                    it: ["Scarpe da trekking", "Zaino con supporto lombare", "Abbigliamento tecnico", "Sacco a pelo", "Lampada frontale"]
                }
            },

            sidebar: {
                duracion: { es: "Rutas de varios días", en: "Multi-day routes", it: "Percorsi di più giorni" },
                epoca: { es: "Según región", en: "Depends on region", it: "Secondo la regione" },
                presupuesto: "Medio",
                intensidad: { es: "Alta", en: "High", it: "Alta" }
            }
        }
    },

    // ==================================================
    // GUÍA 35: VIAJAR EN AVIÓN
    // ==================================================
    {
        id: 35,
        titulo: {
            es: "Qué llevar en el equipaje de mano",
            en: "What to pack in carry-on luggage",
            it: "Cosa portare nel bagaglio a mano"
        },
        categoria: {
            es: "Qué llevar",
            en: "Packing",
            it: "Cosa portare"
        },
        destino: "Avión",
        tipo: {
            es: "Consejos",
            en: "Tips",
            it: "Consigli"
        },
        imagen: "https://images.unsplash.com/photo-1529070538774-1843cb3265df?w=800",
        lectura: "7 min",
        valoracion: "4.7",
        desc: {
            es: "El equipaje de mano es clave para vuelos largos o viajes mochileros. En esta guía te contamos qué llevar contigo, qué no puede faltar y cómo cumplir las normas sin renunciar a lo esencial.",
            en: "Carry-on luggage is key for long flights or backpacking trips. This guide tells you what to bring, what can't be missed, and how to follow regulations without missing essentials.",
            it: "Il bagaglio a mano è fondamentale per voli lunghi o viaggi zaino in spalla. Questa guida spiega cosa portare, cosa non può mancare e come rispettare le regole."
        },
        tags: ["Avión", "Equipaje"],

        detalle: {
            subtitulo: {
                es: "Todo lo importante, siempre contigo.",
                en: "All important items, always with you.",
                it: "Tutto l'importante, sempre con te."
            },
            autor: "Travel Tips",
            intro: {
                es: "Un buen equipaje de mano evita muchos problemas.",
                en: "A good carry-on avoids many problems.",
                it: "Un buon bagaglio a mano evita molti problemi."
            },

            itinerario: [],

            consejos: {
                es: ["Líquidos en formato viaje.", "Documentos siempre accesibles.", "Ropa de emergencia."],
                en: ["Travel-sized liquids.", "Documents always accessible.", "Emergency clothes."],
                it: ["Liquidi formato viaggio.", "Documenti sempre accessibili.", "Abbigliamento di emergenza."]
            },

            maleta: {
                clima: { es: "Interior", en: "Indoor", it: "Interno" },
                items: {
                    es: ["Documentación", "Cargador y batería externa", "Ropa interior de recambio", "Auriculares", "Neceser pequeño"],
                    en: ["Documents", "Charger and power bank", "Spare underwear", "Headphones", "Small toiletry bag"],
                    it: ["Documenti", "Caricatore e power bank", "Biancheria di ricambio", "Cuffie", "Beauty case piccolo"]
                }
            },

            sidebar: {
                duracion: { es: "Vuelos", en: "Flights", it: "Voli" },
                epoca: { es: "Todo el año", en: "All year", it: "Tutto l'anno" },
                presupuesto: "Bajo",
                intensidad: { es: "Baja", en: "Low", it: "Bassa" }
            }
        }
    }

];


// Función auxiliar para obtener datos (por si la necesitamos fuera)
function getGuiaById(id) {
    return guias.find(g => g.id === id);
}