// =====================================================
// 1. PACKS DE RESEÑAS (Para no repetir código)
// =====================================================
const reviewsPack1 = [
    {
        usuario: "Ana P.",
        fecha: { es: "Dic 2024", en: "Dec 2024", it: "Dic 2024" },
        rating: 5,
        comentario: {
            es: "Una estancia inolvidable. El personal fue súper atento y las instalaciones estaban impecables.",
            en: "An unforgettable stay. The staff was super attentive and the facilities were spotless.",
            it: "Un soggiorno indimenticabile. Il personale è stato super attento e le strutture erano impeccabili."
        },
        tipoViaje: { es: "Viaje en pareja", en: "Couples trip", it: "Viaggio di coppia" }
    },
    {
        usuario: "Marc R.",
        fecha: { es: "Nov 2024", en: "Nov 2024", it: "Nov 2024" },
        rating: 4,
        comentario: {
            es: "La ubicación es perfecta, cerca de todo. El desayuno podría mejorar un poco, pero en general muy bien.",
            en: "Location is perfect, close to everything. Breakfast could be a bit better, but overall very good.",
            it: "La posizione è perfetta, vicino a tutto. La colazione potrebbe migliorare un po', ma nel complesso molto bene."
        },
        tipoViaje: { es: "Con amigos", en: "With friends", it: "Con amici" }
    },
    {
        usuario: "Sarah L.",
        fecha: { es: "Oct 2024", en: "Oct 2024", it: "Ott 2024" },
        rating: 5,
        comentario: {
            es: "Relación calidad-precio inmejorable. Volveré seguro.",
            en: "Unbeatable value for money. I will definitely return.",
            it: "Rapporto qualità-prezzo imbattibile. Tornerò sicuramente."
        },
        tipoViaje: { es: "Viaje solo/a", en: "Solo trip", it: "Viaggio in solitaria" }
    }
];

const reviewsPack2 = [
    {
        usuario: "John D.",
        fecha: { es: "Ene 2025", en: "Jan 2025", it: "Gen 2025" },
        rating: 5,
        comentario: {
            es: "Simplemente mágico. Las vistas desde la habitación te dejan sin aliento.",
            en: "Simply magical. The views from the room leave you breathless.",
            it: "Semplicemente magico. Le viste dalla camera ti lasciano senza fiato."
        },
        tipoViaje: { es: "Familia", en: "Family", it: "Famiglia" }
    },
    {
        usuario: "Elena G.",
        fecha: { es: "Dic 2024", en: "Dec 2024", it: "Dic 2024" },
        rating: 4.5,
        comentario: {
            es: "Muy limpio y acogedor. Me sentí como en casa.",
            en: "Very clean and cozy. I felt right at home.",
            it: "Molto pulito e accogliente. Mi sono sentito come a casa."
        },
        tipoViaje: { es: "Viaje en pareja", en: "Couples trip", it: "Viaggio di coppia" }
    },
    {
        usuario: "Carlos M.",
        fecha: { es: "Sep 2024", en: "Sep 2024", it: "Set 2024" },
        rating: 4,
        comentario: {
            es: "Buen servicio y ambiente agradable. Un poco ruidoso por la noche.",
            en: "Good service and nice atmosphere. A bit noisy at night.",
            it: "Buon servizio e atmosfera piacevole. Un po' rumoroso di notte."
        },
        tipoViaje: { es: "Mochilero", en: "Backpacker", it: "Zaino in spalla" }
    }
];

// =====================================================
// 2. LISTA DE ALOJAMIENTOS
// =====================================================

const alojamientos = [
    // ---------------------------------------------------------
    // ÁFRICA
    // ---------------------------------------------------------
    {
        id: 1,
        nombre: "Riad Al Qamar",
        ubicacion: "Barrio de Zamalek, El Cairo, Egipto",
        precio: 72,
        imagenPrincipal: "https://images.unsplash.com/photo-1542314831-068cd1dbfeeb?w=800",
        imagenes: ["https://images.unsplash.com/photo-1542314831-068cd1dbfeeb?w=150"],
        descripcion: {
            es: "Descubre un auténtico oasis de paz en el corazón de El Cairo. Arquitectura tradicional con comodidades modernas.",
            en: "Discover an authentic oasis of peace in the heart of Cairo. Traditional architecture with modern amenities.",
            it: "Scopri un'autentica oasi di pace nel cuore del Cairo. Architettura tradizionale con comfort moderni."
        },
        servicios: { es: ["Wifi gratis", "Piscina", "AC"], en: ["Free Wifi", "Pool", "AC"], it: ["Wifi gratis", "Piscina", "Aria condizionata"] },
        coordenadas: [30.0444, 31.2357],
        rating: 4.7,
        resenas: 123,
        // AÑADIMOS LAS RESEÑAS DETALLADAS AQUÍ:
        listaResenas: reviewsPack1 
    },
    {
        id: 70,
        nombre: "Dar Azul",
        ubicacion: "Medina, Chefchaouen, Marruecos",
        precio: 45,
        imagenPrincipal: "https://images.unsplash.com/photo-1569383746724-6f1b882b8f46?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1569383746724-6f1b882b8f46?w=150"],
        descripcion: {
            es: "Casa tradicional pintada de azul en el corazón de la medina. Terraza con vistas a las montañas del Rif.",
            en: "Traditional blue-painted house in the heart of the medina. Terrace with views of the Rif mountains.",
            it: "Casa tradizionale dipinta di blu nel cuore della medina. Terrazza con vista sulle montagne del Rif."
        },
        servicios: { es: ["Desayuno", "Terraza", "Wifi"], en: ["Breakfast", "Terrace", "Wifi"], it: ["Colazione", "Terrazza", "Wifi"] },
        coordenadas: [35.1716, -5.2697],
        rating: 4.8,
        resenas: 89,
        listaResenas: reviewsPack2
    },
    {
        id: 71,
        nombre: "Table Bay Lodge",
        ubicacion: "Camps Bay, Ciudad del Cabo, Sudáfrica",
        precio: 110,
        imagenPrincipal: "https://plus.unsplash.com/premium_photo-1697730061063-ad499e343f26?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://plus.unsplash.com/premium_photo-1697730061063-ad499e343f26?w=150"],
        descripcion: {
            es: "Alojamiento moderno frente al océano con vistas a la Montaña de la Mesa. Perfecto para amantes de la naturaleza.",
            en: "Modern oceanfront accommodation with views of Table Mountain. Perfect for nature lovers.",
            it: "Alloggio moderno fronte oceano con vista sulla Table Mountain. Perfetto per gli amanti della natura."
        },
        servicios: { es: ["Piscina", "Parking", "Vistas"], en: ["Pool", "Parking", "Views"], it: ["Piscina", "Parcheggio", "Vista"] },
        coordenadas: [-33.9249, 18.4241],
        rating: 4.6,
        resenas: 210,
        listaResenas: reviewsPack1
    },

    // ---------------------------------------------------------
    // EUROPA
    // ---------------------------------------------------------
    {
        id: 10,
        nombre: "Hostal Gaudí",
        ubicacion: "Eixample, Barcelona, España",
        precio: 85,
        imagenPrincipal: "https://images.unsplash.com/photo-1630219694734-fe47ab76b15e?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1551882547-ff40c63fe5fa?w=150"],
        descripcion: {
            es: "A dos calles de la Sagrada Familia. Habitaciones luminosas con balcones modernistas.",
            en: "Two blocks from Sagrada Familia. Bright rooms with modernist balconies.",
            it: "A due isolati dalla Sagrada Familia. Camere luminose con balconi modernisti."
        },
        servicios: { es: ["Wifi gratis", "Desayuno", "Bicis"], en: ["Free Wifi", "Breakfast", "Bikes"], it: ["Wifi gratis", "Colazione", "Bici"] },
        coordenadas: [41.4036, 2.1744],
        rating: 4.5,
        resenas: 210,
        listaResenas: reviewsPack2
    },
    {
        id: 34,
        nombre: "Casa de Triana",
        ubicacion: "Triana, Sevilla, España",
        precio: 65,
        imagenPrincipal: "https://images.unsplash.com/photo-1559564477-6e8582270002?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1559564477-6e8582270002?w=150"],
        descripcion: {
            es: "Auténtica casa andaluza con patio interior lleno de flores. Vive el flamenco y las tapas a pie de calle.",
            en: "Authentic Andalusian house with a flower-filled courtyard. Experience flamenco and tapas nearby.",
            it: "Autentica casa andalusa con cortile pieno di fiori. Vivi il flamenco e le tapas a due passi."
        },
        servicios: { es: ["Patio", "AC", "Cocina"], en: ["Patio", "AC", "Kitchen"], it: ["Cortile", "AC", "Cucina"] },
        coordenadas: [37.3891, -5.9845],
        rating: 4.7,
        resenas: 150,
        listaResenas: reviewsPack1
    },
    {
        id: 20,
        nombre: "Le Petit Parisien",
        ubicacion: "Montmartre, París, Francia",
        precio: 110,
        imagenPrincipal: "https://plus.unsplash.com/premium_photo-1661919210043-fd847a58522d?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://plus.unsplash.com/premium_photo-1661919210043-fd847a58522d?w=150"],
        descripcion: {
            es: "Encanto bohemio a pasos del Sacré-Cœur. Habitaciones románticas con vistas a los tejados de París.",
            en: "Bohemian charm steps from Sacré-Cœur. Romantic rooms with views of Paris rooftops.",
            it: "Fascino bohémien a due passi dal Sacré-Cœur. Camere romantiche con vista sui tetti di Parigi."
        },
        servicios: { es: ["Wifi", "Desayuno", "Ascensor"], en: ["Wifi", "Breakfast", "Elevator"], it: ["Wifi", "Colazione", "Ascensore"] },
        coordenadas: [48.8867, 2.3431],
        rating: 4.6,
        resenas: 300,
        listaResenas: reviewsPack2
    },
    {
        id: 30,
        nombre: "Hotel Sacher Wien",
        ubicacion: "Innere Stadt, Viena, Austria",
        precio: 150,
        imagenPrincipal: "https://images.unsplash.com/photo-1731223832507-ebe5373129e6?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1731223832507-ebe5373129e6?w=150"],
        descripcion: {
            es: "Elegancia clásica frente a la Ópera. Disfruta de la famosa tarta Sacher en nuestro café histórico.",
            en: "Classic elegance opposite the Opera. Enjoy the famous Sacher cake in our historic café.",
            it: "Eleganza classica di fronte all'Opera. Goditi la famosa torta Sacher nel nostro caffè storico."
        },
        servicios: { es: ["Spa", "Restaurante", "Lujo"], en: ["Spa", "Restaurant", "Luxury"], it: ["Spa", "Ristorante", "Lusso"] },
        coordenadas: [48.2082, 16.3738],
        rating: 4.9,
        resenas: 540,
        listaResenas: reviewsPack1
    },
    {
        id: 31,
        nombre: "Canal House Bruges",
        ubicacion: "Centro, Brujas, Bélgica",
        precio: 95,
        imagenPrincipal: "https://plus.unsplash.com/premium_photo-1661963659103-9602b3811297?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://plus.unsplash.com/premium_photo-1661963659103-9602b3811297?w=150"],
        descripcion: {
            es: "Bed & Breakfast romántico junto a los canales. Edificio medieval restaurado con mucho encanto.",
            en: "Romantic Bed & Breakfast by the canals. Restored medieval building with lots of charm.",
            it: "Bed & Breakfast romantico lungo i canali. Edificio medievale restaurato con molto fascino."
        },
        servicios: { es: ["Desayuno", "Vistas canal"], en: ["Breakfast", "Canal view"], it: ["Colazione", "Vista canale"] },
        coordenadas: [51.2093, 3.2247],
        rating: 4.8,
        resenas: 220,
        listaResenas: reviewsPack2
    },
    {
        id: 38,
        nombre: "Trastevere Home",
        ubicacion: "Trastevere, Roma, Italia",
        precio: 85,
        imagenPrincipal: "https://images.unsplash.com/photo-1529154036614-a60975f5c760?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1529154036614-a60975f5c760?w=150"],
        descripcion: {
            es: "Apartamento acogedor en el barrio más vibrante de Roma. Rodeado de trattorias y callejuelas empedradas.",
            en: "Cozy apartment in Rome's most vibrant neighborhood. Surrounded by trattorias and cobblestone streets.",
            it: "Accogliente appartamento nel quartiere più vivace di Roma. Circondato da trattorie e vicoli acciottolati."
        },
        servicios: { es: ["Cocina", "Wifi", "Céntrico"], en: ["Kitchen", "Wifi", "Central"], it: ["Cucina", "Wifi", "Centrale"] },
        coordenadas: [41.8883, 12.4701],
        rating: 4.5,
        resenas: 310,
        listaResenas: reviewsPack1
    },
    {
        id: 37,
        nombre: "Artsy Loft Florence",
        ubicacion: "Santa Croce, Florencia, Italia",
        precio: 100,
        imagenPrincipal: "https://images.unsplash.com/photo-1476362174823-3a23f4aa6d76?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1476362174823-3a23f4aa6d76?w=150"],
        descripcion: {
            es: "Loft de diseño a pasos del Duomo. Perfecto para amantes del arte y la arquitectura renacentista.",
            en: "Design loft steps from the Duomo. Perfect for lovers of art and Renaissance architecture.",
            it: "Loft di design a due passi dal Duomo. Perfetto per gli amanti dell'arte e dell'architettura rinascimentale."
        },
        servicios: { es: ["AC", "Wifi", "Cocina"], en: ["AC", "Wifi", "Kitchen"], it: ["AC", "Wifi", "Cucina"] },
        coordenadas: [43.7696, 11.2558],
        rating: 4.7,
        resenas: 180,
        listaResenas: reviewsPack2
    },
    {
        id: 39,
        nombre: "Venetian Palace",
        ubicacion: "San Marco, Venecia, Italia",
        precio: 180,
        imagenPrincipal: "https://plus.unsplash.com/premium_photo-1661963047742-dabc5a735357?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://plus.unsplash.com/premium_photo-1661963047742-dabc5a735357?w=150"],
        descripcion: {
            es: "Habitaciones con vistas al Gran Canal. Decoración clásica veneciana y muelle privado.",
            en: "Rooms with views of the Grand Canal. Classic Venetian decor and private dock.",
            it: "Camere con vista sul Canal Grande. Arredamento classico veneziano e molo privato."
        },
        servicios: { es: ["Vistas", "Barco", "Desayuno"], en: ["Views", "Boat", "Breakfast"], it: ["Vista", "Barca", "Colazione"] },
        coordenadas: [45.4408, 12.3155],
        rating: 4.6,
        resenas: 400,
        listaResenas: reviewsPack1
    },
    {
        id: 42,
        nombre: "Canal Houseboat",
        ubicacion: "Jordaan, Ámsterdam, Países Bajos",
        precio: 130,
        imagenPrincipal: "https://plus.unsplash.com/premium_photo-1661878122586-2d75a86f3400?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://plus.unsplash.com/premium_photo-1661878122586-2d75a86f3400?w=150"],
        descripcion: {
            es: "Duerme sobre el agua en una casa flotante moderna. Experiencia única en los canales de Ámsterdam.",
            en: "Sleep on the water in a modern houseboat. Unique experience on the canals of Amsterdam.",
            it: "Dormi sull'acqua in una moderna casa galleggiante. Esperienza unica sui canali di Amsterdam."
        },
        servicios: { es: ["Terraza", "Cocina", "Wifi"], en: ["Terrace", "Kitchen", "Wifi"], it: ["Terrazza", "Cucina", "Wifi"] },
        coordenadas: [52.3676, 4.9041],
        rating: 4.8,
        resenas: 250,
        listaResenas: reviewsPack2
    },
    {
        id: 44,
        nombre: "Soho Studio",
        ubicacion: "Soho, Londres, Reino Unido",
        precio: 140,
        imagenPrincipal: "https://images.unsplash.com/photo-1513635269975-59663e0ac1ad?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1513635269975-59663e0ac1ad?w=150"],
        descripcion: {
            es: "Estudio moderno en el corazón del entretenimiento de Londres. Teatros, bares y tiendas a la vuelta de la esquina.",
            en: "Modern studio in the heart of London's entertainment. Theatres, bars, and shops around the corner.",
            it: "Studio moderno nel cuore dell'intrattenimento di Londra. Teatri, bar e negozi dietro l'angolo."
        },
        servicios: { es: ["Wifi", "Gym", "Céntrico"], en: ["Wifi", "Gym", "Central"], it: ["Wifi", "Palestra", "Centrale"] },
        coordenadas: [51.5074, -0.1278],
        rating: 4.4,
        resenas: 320,
        listaResenas: reviewsPack1
    },
    {
        id: 46,
        nombre: "Charles Bridge Apt",
        ubicacion: "Malá Strana, Praga, República Checa",
        precio: 80,
        imagenPrincipal: "https://plus.unsplash.com/premium_photo-1661963067279-2f7bf970c49c?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://plus.unsplash.com/premium_photo-1661963067279-2f7bf970c49c?w=150"],
        descripcion: {
            es: "Apartamento histórico con vistas al Puente de Carlos. Techos altos y atmósfera romántica.",
            en: "Historic apartment overlooking Charles Bridge. High ceilings and romantic atmosphere.",
            it: "Appartamento storico con vista sul Ponte Carlo. Soffitti alti e atmosfera romantica."
        },
        servicios: { es: ["Vistas", "Cocina", "Wifi"], en: ["Views", "Kitchen", "Wifi"], it: ["Vista", "Cucina", "Wifi"] },
        coordenadas: [50.0755, 14.4378],
        rating: 4.7,
        resenas: 190,
        listaResenas: reviewsPack2
    },
    {
        id: 32,
        nombre: "Hygge House",
        ubicacion: "Nyhavn, Copenhague, Dinamarca",
        precio: 125,
        imagenPrincipal: "https://plus.unsplash.com/premium_photo-1691414363231-836e2e1bf0ed?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://plus.unsplash.com/premium_photo-1691414363231-836e2e1bf0ed?w=150"],
        descripcion: {
            es: "Diseño nórdico minimalista y acogedor. Situado en el icónico puerto de colores de Nyhavn.",
            en: "Minimalist and cozy Nordic design. Located in the iconic colorful harbor of Nyhavn.",
            it: "Design nordico minimalista e accogliente. Situato nell'iconico porto colorato di Nyhavn."
        },
        servicios: { es: ["Diseño", "Bicis", "Café"], en: ["Design", "Bikes", "Coffee"], it: ["Design", "Bici", "Caffè"] },
        coordenadas: [55.6761, 12.5683],
        rating: 4.6,
        resenas: 140,
        listaResenas: reviewsPack1
    },
    {
        id: 48,
        nombre: "Zurich Lake View",
        ubicacion: "Seefeld, Zúrich, Suiza",
        precio: 190,
        imagenPrincipal: "https://images.unsplash.com/photo-1620563092215-0fbc6b55cfc5?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1620563092215-0fbc6b55cfc5?w=150"],
        descripcion: {
            es: "Lujo y tranquilidad frente al lago de Zúrich. Habitaciones elegantes cerca de la ópera.",
            en: "Luxury and tranquility facing Lake Zurich. Elegant rooms near the opera.",
            it: "Lusso e tranquillità di fronte al lago di Zurigo. Camere eleganti vicino all'opera."
        },
        servicios: { es: ["Lujo", "Vistas", "Spa"], en: ["Luxury", "Views", "Spa"], it: ["Lusso", "Vista", "Spa"] },
        coordenadas: [47.3769, 8.5417],
        rating: 4.9,
        resenas: 90,
        listaResenas: reviewsPack2
    },
    {
        id: 43,
        nombre: "Ribeira Guesthouse",
        ubicacion: "Ribeira, Oporto, Portugal",
        precio: 75,
        imagenPrincipal: "https://plus.unsplash.com/premium_photo-1677344087971-91eee10dfeb1?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://plus.unsplash.com/premium_photo-1677344087971-91eee10dfeb1?w=150"],
        descripcion: {
            es: "Casa de huéspedes con encanto y vistas al río Duero. Cerca de las bodegas de vino de Oporto.",
            en: "Charming guesthouse with views of the Douro River. Close to the Port wine cellars.",
            it: "Pensione di charme con vista sul fiume Douro. Vicino alle cantine di vino Porto."
        },
        servicios: { es: ["Vistas río", "Desayuno", "Wifi"], en: ["River view", "Breakfast", "Wifi"], it: ["Vista fiume", "Colazione", "Wifi"] },
        coordenadas: [41.1579, -8.6291],
        rating: 4.6,
        resenas: 230,
        listaResenas: reviewsPack1
    },
    {
        id: 49,
        nombre: "Aurora Cabin",
        ubicacion: "Afueras, Reikiavik, Islandia",
        precio: 200,
        imagenPrincipal: "https://plus.unsplash.com/premium_photo-1661962984700-16b03ecda58a?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://plus.unsplash.com/premium_photo-1661962984700-16b03ecda58a?w=150"],
        descripcion: {
            es: "Cabaña de cristal perfecta para ver auroras boreales. Jacuzzi exterior y privacidad total.",
            en: "Glass cabin perfect for viewing northern lights. Outdoor jacuzzi and total privacy.",
            it: "Cabina di vetro perfetta per vedere l'aurora boreale. Jacuzzi esterna e privacy totale."
        },
        servicios: { es: ["Jacuzzi", "Vistas", "Parking"], en: ["Jacuzzi", "Views", "Parking"], it: ["Jacuzzi", "Vista", "Parcheggio"] },
        coordenadas: [64.1466, -21.9426],
        rating: 4.9,
        resenas: 85,
        listaResenas: reviewsPack2
    },

    // ---------------------------------------------------------
    // ASIA
    // ---------------------------------------------------------
    {
        id: 54,
        nombre: "Ryokan Zen",
        ubicacion: "Gion, Kioto, Japón",
        precio: 250,
        imagenPrincipal: "https://images.unsplash.com/photo-1493976040374-85c8e12f0c0e?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1493976040374-85c8e12f0c0e?w=150"],
        descripcion: {
            es: "Alojamiento tradicional japonés con tatami y onsen privado. Cena kaiseki incluida.",
            en: "Traditional Japanese accommodation with tatami and private onsen. Kaiseki dinner included.",
            it: "Alloggio tradizionale giapponese con tatami e onsen privato. Cena kaiseki inclusa."
        },
        servicios: { es: ["Onsen", "Cena", "Jardín"], en: ["Onsen", "Dinner", "Garden"], it: ["Onsen", "Cena", "Giardino"] },
        coordenadas: [35.0116, 135.7681],
        rating: 4.9,
        resenas: 400,
        listaResenas: reviewsPack1
    },
    {
        id: 59,
        nombre: "Lanna Resort",
        ubicacion: "Montañas, Chiang Mai, Tailandia",
        precio: 55,
        imagenPrincipal: "https://plus.unsplash.com/premium_photo-1661929242720-140374d97c94?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://plus.unsplash.com/premium_photo-1661929242720-140374d97c94?w=150"],
        descripcion: {
            es: "Resort ecológico rodeado de arrozales y templos. Paz y naturaleza en el norte de Tailandia.",
            en: "Eco-resort surrounded by rice fields and temples. Peace and nature in northern Thailand.",
            it: "Eco-resort circondato da risaie e templi. Pace e natura nel nord della Thailandia."
        },
        servicios: { es: ["Piscina", "Masajes", "Yoga"], en: ["Pool", "Massage", "Yoga"], it: ["Piscina", "Massaggi", "Yoga"] },
        coordenadas: [18.7883, 98.9853],
        rating: 4.7,
        resenas: 310,
        listaResenas: reviewsPack2
    },
    {
        id: 61,
        nombre: "Marina Bay View",
        ubicacion: "Marina Bay, Singapur",
        precio: 300,
        imagenPrincipal: "https://images.unsplash.com/photo-1508964942454-1a56651d54ac?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1508964942454-1a56651d54ac?w=150"],
        descripcion: {
            es: "Lujo futurista con la piscina infinita más famosa del mundo. Vistas increíbles de la ciudad.",
            en: "Futuristic luxury with the world's most famous infinity pool. Incredible city views.",
            it: "Lusso futuristico con la piscina a sfioro più famosa al mondo. Vista incredibile sulla città."
        },
        servicios: { es: ["Piscina infinita", "Lujo", "Casino"], en: ["Infinity pool", "Luxury", "Casino"], it: ["Piscina a sfioro", "Lusso", "Casinò"] },
        coordenadas: [1.2868, 103.8545],
        rating: 4.8,
        resenas: 900,
        listaResenas: reviewsPack1
    },
    {
        id: 50,
        nombre: "Hutong Courtyard",
        ubicacion: "Dongcheng, Beijing, China",
        precio: 90,
        imagenPrincipal: "https://plus.unsplash.com/premium_photo-1723433351351-0f6cd5d21537?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://plus.unsplash.com/premium_photo-1723433351351-0f6cd5d21537?w=150"],
        descripcion: {
            es: "Vive la historia en una casa patio tradicional renovada. Tranquilidad en medio de la ciudad.",
            en: "Live history in a renovated traditional courtyard house. Tranquility in the middle of the city.",
            it: "Vivi la storia in una casa a corte tradizionale rinnovata. Tranquillità in mezzo alla città."
        },
        servicios: { es: ["Jardín", "Desayuno", "Wifi"], en: ["Garden", "Breakfast", "Wifi"], it: ["Giardino", "Colazione", "Wifi"] },
        coordenadas: [39.9042, 116.4074],
        rating: 4.6,
        resenas: 150,
        listaResenas: reviewsPack2
    },

    // ---------------------------------------------------------
    // AMÉRICA
    // ---------------------------------------------------------
    {
        id: 92,
        nombre: "Brooklyn Loft",
        ubicacion: "Williamsburg, Nueva York, EEUU",
        precio: 160,
        imagenPrincipal: "https://images.unsplash.com/photo-1485871981521-5b1fd3805eee?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1485871981521-5b1fd3805eee?w=150"],
        descripcion: {
            es: "Loft industrial con vistas a Manhattan. Rodeado de cafeterías de moda y arte urbano.",
            en: "Industrial loft with views of Manhattan. Surrounded by trendy cafes and street art.",
            it: "Loft industriale con vista su Manhattan. Circondato da caffè alla moda e arte urbana."
        },
        servicios: { es: ["Wifi", "Cocina", "Vistas"], en: ["Wifi", "Kitchen", "Views"], it: ["Wifi", "Cucina", "Vista"] },
        coordenadas: [40.7128, -74.0060],
        rating: 4.5,
        resenas: 340,
        listaResenas: reviewsPack1
    },
    {
        id: 102,
        nombre: "Casa Colonial Cartagena",
        ubicacion: "Ciudad Amurallada, Cartagena, Colombia",
        precio: 120,
        imagenPrincipal: "https://images.unsplash.com/photo-1583531352515-8884af319dc1?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1583531352515-8884af319dc1?w=150"],
        descripcion: {
            es: "Casona histórica con piscina privada y balcones floridos. En el corazón de la ciudad vieja.",
            en: "Historical mansion with private pool and flowery balconies. In the heart of the old city.",
            it: "Dimora storica con piscina privata e balconi fioriti. Nel cuore della città vecchia."
        },
        servicios: { es: ["Piscina", "AC", "Desayuno"], en: ["Pool", "AC", "Breakfast"], it: ["Piscina", "AC", "Colazione"] },
        coordenadas: [10.3910, -75.4794],
        rating: 4.8,
        resenas: 210,
        listaResenas: reviewsPack2
    },
    {
        id: 104,
        nombre: "Inca Stone House",
        ubicacion: "San Blas, Cuzco, Perú",
        precio: 55,
        imagenPrincipal: "https://images.unsplash.com/photo-1609944433409-81bda5323abc?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1609944433409-81bda5323abc?w=150"],
        descripcion: {
            es: "Alojamiento con muros incas originales. Vistas panorámicas a la Plaza de Armas.",
            en: "Accommodation with original Inca walls. Panoramic views of the Plaza de Armas.",
            it: "Alloggio con mura inca originali. Vista panoramica sulla Plaza de Armas."
        },
        servicios: { es: ["Calefacción", "Desayuno", "Tours"], en: ["Heating", "Breakfast", "Tours"], it: ["Riscaldamento", "Colazione", "Tour"] },
        coordenadas: [-13.5167, -71.9772],
        rating: 4.6,
        resenas: 280,
        listaResenas: reviewsPack1
    },
    {
        id: 101,
        nombre: "Copacabana Flat",
        ubicacion: "Copacabana, Río de Janeiro, Brasil",
        precio: 85,
        imagenPrincipal: "https://plus.unsplash.com/premium_photo-1671211307997-f4f552b0601c?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://plus.unsplash.com/premium_photo-1671211307997-f4f552b0601c?w=150"],
        descripcion: {
            es: "Apartamento luminoso a una calle de la playa. Vistas al mar y fácil acceso a Ipanema.",
            en: "Bright apartment one block from the beach. Ocean views and easy access to Ipanema.",
            it: "Appartamento luminoso a un isolato dalla spiaggia. Vista mare e facile accesso a Ipanema."
        },
        servicios: { es: ["AC", "Cocina", "Playa"], en: ["AC", "Kitchen", "Beach"], it: ["AC", "Cucina", "Spiaggia"] },
        coordenadas: [-22.9068, -43.1729],
        rating: 4.5,
        resenas: 190,
        listaResenas: reviewsPack2
    },

    // ---------------------------------------------------------
    // OCEANÍA
    // ---------------------------------------------------------
    {
        id: 80,
        nombre: "Bondi Beach Pad",
        ubicacion: "Bondi, Sídney, Australia",
        precio: 140,
        imagenPrincipal: "https://images.unsplash.com/photo-1506973035872-a4ec16b8e8d9?ixlib=rb-4.1.0&w=800",
        imagenes: ["https://images.unsplash.com/photo-1506973035872-a4ec16b8e8d9?w=150"],
        descripcion: {
            es: "Apartamento surfero a pasos de la arena. Relájate con el sonido del océano.",
            en: "Surfer apartment steps from the sand. Relax with the sound of the ocean.",
            it: "Appartamento per surfisti a due passi dalla sabbia. Rilassati con il suono dell'oceano."
        },
        servicios: { es: ["Wifi", "Tablas surf", "Cocina"], en: ["Wifi", "Surfboards", "Kitchen"], it: ["Wifi", "Tavole surf", "Cucina"] },
        coordenadas: [-33.8688, 151.2093],
        rating: 4.7,
        resenas: 160,
        listaResenas: reviewsPack1
    }
];